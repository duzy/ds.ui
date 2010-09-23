/**
 *    Copyright 2010-09-21 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/graphics/region.hpp>
#include <algorithm>
#include <iterator>
#include <stdint.h>
//#include <sys/types.h>

namespace ds { namespace graphics {

    // from android:frameworks/base/include/private/ui/RegionHelper.h
    template<typename RECT>
    class region_operator
    {
      //typedef typename RECT::value_type TYPE;
      typedef coordinate_t TYPE;
      static const TYPE max_value = 0x7FFFFFF;

    public:
      /* 
       * Common boolean operations:
       * value is computed as 0b101 op 0b110
       *    other boolean operation are possible, simply compute
       *    their corresponding value with the above formulae and use
       *    it when instantiating a region_operator.
       */
      static const uint32_t LHS = 0x5;  // 0b101
      static const uint32_t RHS = 0x6;  // 0b110
      enum {
        op_nand = LHS & ~RHS,
        op_and  = LHS &  RHS,
        op_or   = LHS |  RHS,
        op_xor  = LHS ^  RHS
      };

      struct region {
        RECT const* rects;
        size_t count;
        TYPE dx;
        TYPE dy;
        inline region(const region& rhs) 
          : rects(rhs.rects), count(rhs.count), dx(rhs.dx), dy(rhs.dy) { }
        inline region(RECT const* r, size_t c) 
          : rects(r), count(c), dx(), dy() { }
        inline region(RECT const* r, size_t c, TYPE dx, TYPE dy) 
          : rects(r), count(c), dx(dx), dy(dy) { }
      };

      class region_rasterizer {
        friend class region_operator;
        virtual void operator()(const RECT& rect) = 0;
      public:
        virtual ~region_rasterizer() { };
      };
    
      inline region_operator(int op, const region& lhs, const region& rhs) 
        : op_mask(op), spanner(lhs, rhs) 
      {
      }

      void operator()(region_rasterizer& rasterizer) {
        RECT current;
        do {
          TYPE top(current.top()), bot(current.bottom());
          SpannerInner spannerInner(spanner.lhs, spanner.rhs);
          //int inside = spanner.next(current.top(), current.bottom());
          int inside = spanner.next(top, bot);
          current.top(top), current.bottom(bot);
          spannerInner.prepare(inside);
          do {
            //TYPE left, right;
            //int inside = spannerInner.next(current.left(), current.right());
            TYPE left(current.left()), right(current.right());
            int inside = spannerInner.next(left, right);
            current.left(left), current.right(right);
            if ((op_mask >> inside) & 1) {
              if (current.left() < current.right() && 
                  current.top() < current.bottom()) {
                rasterizer(current);
              }
            }
          } while(!spannerInner.isDone());
        } while(!spanner.isDone());
      }

    private:    
      uint32_t op_mask;

      class SpannerBase
      {
      public:
        enum {
          lhs_before_rhs   = 0,
          lhs_after_rhs    = 1,
          lhs_coincide_rhs = 2
        };

      protected:
        TYPE lhs_head;
        TYPE lhs_tail;
        TYPE rhs_head;
        TYPE rhs_tail;

        inline int next(TYPE& head, TYPE& tail,
                        bool& more_lhs, bool& more_rhs) 
        {
          int inside;
          more_lhs = false;
          more_rhs = false;
          if (lhs_head < rhs_head) {
            inside = lhs_before_rhs;
            head = lhs_head;
            if (lhs_tail <= rhs_head) {
              tail = lhs_tail;
              more_lhs = true;
            } else {
              lhs_head = rhs_head;
              tail = rhs_head;
            }
          } else if (rhs_head < lhs_head) {
            inside = lhs_after_rhs;
            head = rhs_head;
            if (rhs_tail <= lhs_head) {
              tail = rhs_tail;
              more_rhs = true;
            } else {
              rhs_head = lhs_head;
              tail = lhs_head;
            }
          } else {
            inside = lhs_coincide_rhs;
            head = lhs_head;
            if (lhs_tail <= rhs_tail) {
              tail = rhs_head = lhs_tail;
              more_lhs = true;
            }
            if (rhs_tail <= lhs_tail) {
              tail = lhs_head = rhs_tail;
              more_rhs = true;
            }
          }
          return inside;
        }
      };

      class Spanner : protected SpannerBase 
      {
        friend class region_operator;
        region lhs;
        region rhs;

      public:
        inline Spanner(const region& lhs, const region& rhs)
          : lhs(lhs), rhs(rhs) 
        {
          SpannerBase::lhs_head = lhs.rects->top()      + lhs.dy;
          SpannerBase::lhs_tail = lhs.rects->bottom()   + lhs.dy;
          SpannerBase::rhs_head = rhs.rects->top()      + rhs.dy;
          SpannerBase::rhs_tail = rhs.rects->bottom()   + rhs.dy;
        }

        inline bool isDone() const {
          return !rhs.count && !lhs.count;
        }

        inline int next(TYPE& top, TYPE& bottom) 
        {
          bool more_lhs = false;
          bool more_rhs = false;
          int inside = SpannerBase::next(top, bottom, more_lhs, more_rhs);
          if (more_lhs) {
            advance(lhs, SpannerBase::lhs_head, SpannerBase::lhs_tail);
          }
          if (more_rhs) {
            advance(rhs, SpannerBase::rhs_head, SpannerBase::rhs_tail);
          }
          return inside;
        }

      private:
        static inline 
        void advance(region& reg, TYPE& aTop, TYPE& aBottom) {
          // got to next span
          size_t count = reg.count;
          RECT const * rects = reg.rects;
          RECT const * const end = rects + count;
          const int top = rects->top();
          while (rects != end && rects->top() == top) {
            rects++;
            count--;
          }
          if (rects != end) {
            aTop    = rects->top()    + reg.dy;
            aBottom = rects->bottom() + reg.dy;
          } else {
            aTop    = max_value;
            aBottom = max_value;
          }
          reg.rects = rects;
          reg.count = count;
        }
      };

      class SpannerInner : protected SpannerBase 
      {
        region lhs;
        region rhs;
        
      public:
        inline SpannerInner(const region& lhs, const region& rhs)
          : lhs(lhs), rhs(rhs) 
        {
        }

        inline void prepare(int inside) {
          if (inside == SpannerBase::lhs_before_rhs) {
            SpannerBase::lhs_head = lhs.rects->left()  + lhs.dx;
            SpannerBase::lhs_tail = lhs.rects->right() + lhs.dx;
            SpannerBase::rhs_head = max_value;
            SpannerBase::rhs_tail = max_value;
          } else if (inside == SpannerBase::lhs_after_rhs) {
            SpannerBase::lhs_head = max_value;
            SpannerBase::lhs_tail = max_value;
            SpannerBase::rhs_head = rhs.rects->left()  + rhs.dx;
            SpannerBase::rhs_tail = rhs.rects->right() + rhs.dx;
          } else {
            SpannerBase::lhs_head = lhs.rects->left()  + lhs.dx;
            SpannerBase::lhs_tail = lhs.rects->right() + lhs.dx;
            SpannerBase::rhs_head = rhs.rects->left()  + rhs.dx;
            SpannerBase::rhs_tail = rhs.rects->right() + rhs.dx;
          }
        }

        inline bool isDone() const {
          return SpannerBase::lhs_head == max_value && 
            SpannerBase::rhs_head == max_value;
        }

        inline int next(TYPE& left, TYPE& right) 
        {
          bool more_lhs = false;
          bool more_rhs = false;
          int inside = SpannerBase::next(left, right, more_lhs, more_rhs);
          if (more_lhs) {
            advance(lhs, SpannerBase::lhs_head, SpannerBase::lhs_tail);
          }
          if (more_rhs) {
            advance(rhs, SpannerBase::rhs_head, SpannerBase::rhs_tail);
          }
          return inside;
        }

      private:
        static inline 
        void advance(region& reg, TYPE& left, TYPE& right) {
          if (reg.rects && reg.count) {
            const int cur_span_top = reg.rects->top();
            reg.rects++;
            reg.count--;
            if (!reg.count || reg.rects->top() != cur_span_top) {
              left  = max_value;
              right = max_value;
            } else {
              left  = reg.rects->left()  + reg.dx;
              right = reg.rects->right() + reg.dx;
            }
          }
        }
      };

      Spanner spanner;
    };//class region_operator

    // This is our region rasterizer, which merges rects and spans together
    // to obtain an optimal region.
    class region::rasterizer : public region_operator<box>::region_rasterizer 
    {
      box& bounds;
      std::vector<box>& storage;

      box* head;
      box* tail;
      std::vector<box> span;
      box* cur;

    public:
      rasterizer(region& reg) 
        : bounds(reg._bounds), storage(reg._boxes), head(), tail(), cur() {
        bounds.top(0), bounds.bottom(0);
        bounds.left(INT_MAX), bounds.right(INT_MIN);
        storage.clear();
      }

      ~rasterizer() {
        if (span.size()) {
          flushSpan();
        }
        if (storage.size()) {
          bounds.top( storage[0].top() );
          bounds.bottom( storage.back().bottom() ); //storage.top().bottom;
          if (storage.size() == 1) {
            storage.clear();
          }
        } else {
          bounds.left( 0 );
          bounds.right( 0 );
        }
      }
    
      virtual void operator()(const box& rect) {
        //LOGD(">>> %3d, %3d, %3d, %3d", 
        //        rect.left, rect.top, rect.right, rect.bottom);
        if (span.size()) {
          if (cur->top() != rect.top()) {
            flushSpan();
          } else if (cur->right() == rect.left()) {
            cur->right( rect.right() );
            return;
          }
        }
        span.push_back(rect); //span.add(rect);
        cur = &span[0] + (span.size() - 1);
      }
    private:
      template<typename T> 
      static inline T min(T rhs, T lhs) { return rhs < lhs ? rhs : lhs; }
      template<typename T> 
      static inline T max(T rhs, T lhs) { return rhs > lhs ? rhs : lhs; }
      void flushSpan() {
        bool merge = false;
        if (tail-head == ssize_t(span.size())) {
          box const* p = cur;
          box const* q = head;
          if (p->top() == q->bottom()) {
            merge = true;
            while (q != tail) {
              if ((p->left() != q->left()) || (p->right() != q->right())) {
                merge = false;
                break;
              }
              p++, q++;
            }
          }
        }
        if (merge) {
          const int bottom = span[0].bottom();
          box* r = head;
          while (r != tail) {
            r->bottom( bottom );
            r++;
          }
        } else {
          bounds.left( min(span.front().left(), bounds.left()) );
          bounds.right( max(span.back().right(), bounds.right()) );
          //storage.appendVector(span);
          std::copy( span.begin(), span.end(), std::back_inserter(storage) );
          //tail = storage.editArray() + storage.size();
          tail = &storage[0] + storage.size();
          head = tail - span.size();
        }
        span.clear();
      }
    };//class region::rasterizer

    class region::operation
    {
      static box const * const get_array( const region & r, size_t * c )
      {
        if ( r._boxes.empty() ) {
          if (c) *c = 1;
          return &r._bounds;
        } else {
          if (c) *c = r._boxes.size();
          return &r._boxes[0];
        }
      }

    public:
      static void boolean_operation(int op, region& dst,
                                    const region& lhs,
                                    const region& rhs, int dx, int dy)
      {
        size_t lhs_count;
        box const * const lhs_rects = get_array(lhs, &lhs_count);

        size_t rhs_count;
        box const * const rhs_rects = get_array(rhs, &rhs_count);

        region_operator<box>::region lhs_region(lhs_rects, lhs_count);
        region_operator<box>::region rhs_region(rhs_rects, rhs_count, dx, dy);
        region_operator<box> oper(op, lhs_region, rhs_region);
        { // scope for rasterizer (dtor has side effects)
          rasterizer r(dst);
          oper(r);
        }
      }

      static void boolean_operation(int op, region& dst,
                                    const region& lhs,
                                    const box& rhs, int dx, int dy)
      {
#if VALIDATE_WITH_CORECG || VALIDATE_REGIONS
        boolean_operation(op, dst, lhs, region(rhs), dx, dy);
#else
        size_t lhs_count;
        box const * const lhs_rects = get_array(lhs, &lhs_count);

        region_operator<box>::region lhs_region(lhs_rects, lhs_count);
        region_operator<box>::region rhs_region(&rhs, 1, dx, dy);
        region_operator<box> oper(op, lhs_region, rhs_region);
        { // scope for rasterizer (dtor has side effects)
          rasterizer r(dst);
          oper(r);
        }
#endif
      }
    };//class region::operation

    //=======================================================================
    
    region::region()
      : _bounds()
      , _boxes()
    {
    }

    region::region( const region & o )
      : _bounds( o._bounds )
      , _boxes( o._boxes )
    {
    }

    region::region( const box & b )
      : _bounds(b)
      , _boxes()
    {
    }

    region & region::operator |= ( const box & rhs )
    {
      region lhs(*this);
      operation::boolean_operation( region_operator<box>::op_or, *this, lhs, rhs, 0, 0 );
      return *this;
    }

    region & region::operator &= ( const box & rhs )
    {
      region lhs(*this);
      operation::boolean_operation( region_operator<box>::op_and, *this, lhs, rhs, 0, 0 );
      return *this;
    }

    region & region::operator -= ( const box & rhs )
    {
      region lhs(*this);
      operation::boolean_operation( region_operator<box>::op_nand, *this, lhs, rhs, 0, 0 );
      return *this;
    }

    const region region::operator | ( const box & rhs ) const
    {
      region res;
      operation::boolean_operation( region_operator<box>::op_or, res, *this, rhs, 0, 0 );
      return res;
    }

    const region region::operator & ( const box & rhs ) const
    {
      region res;
      operation::boolean_operation( region_operator<box>::op_and, res, *this, rhs, 0, 0 );
      return res;
    }

    const region region::operator - ( const box & rhs ) const
    {
      region res;
      operation::boolean_operation( region_operator<box>::op_nand, res, *this, rhs, 0, 0 );
      return res;
    }

    region & region::operator |= ( const region & )
    {
      return *this;
    }

    region & region::operator &= ( const region & )
    {
      return *this;
    }

    region & region::operator -= ( const region & )
    {
      return *this;
    }

    const region region::operator | ( const region & ) const
    {
      region r;
      return r;
    }

    const region region::operator & ( const region & ) const
    {
      region r;
      return r;
    }

    const region region::operator - ( const region & ) const
    {
      region r;
      return r;
    }

    
  }//namespace graphics
}//namespace ds

