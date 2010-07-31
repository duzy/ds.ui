#ifndef __DS_GRAPHICS_RECT__hpp____by_Duzy_Chan__
#define __DS_GRAPHICS_RECT__hpp____by_Duzy_Chan__ 1
#       include <boost/cstdint.hpp>

namespace ds { namespace graphics {

    /*
    template<typename T>
    struct rect
    {
      typedef T scalar_t;
      scalar_t left, top, right, bottom;

      void set(scalar_t l, scalar_t t, scalar_t r, scalar_t b)
      {
        left = l, top = t, right = r, bottom = b;
      }

      void set_xywh(scalar_t x, scalar_t y, scalar_t w, scalar_t h)
      {
        set(x, y, x+w, y+h);
      }

      //void set_empty() { std::memset(this, 0, sizeof(*this)); }
      void set_empty() { left = top = right = bottom = 0; }

      bool is_empty() const { return left >= right || top >= bottom; }
      
      scalar_t width() const { return right - left; }
      scalar_t height() const { return bottom - top; }

      void offset(scalar_t dx, scalar_t dy)
      {
        left += dx, top += dy, right += dx, bottom += dy;
      }

      void inset(scalar_t dx, scalar_t dy)
      {
        left += dx, top += dy, right -= dx, bottom -= dy;
      }

      rect intersect(const rect & r) const
      {
        rect o;                    o.set_empty();
        if ( r.is_empty() ) return o;
        o.left = left < r.left ? r.left : left;
        o.top  = top  < r.top  ? r.top  : top;
        o.right  = right  < r.right ? right : r.right;
        o.bottom = bottom < r.bottom ? bottom : r.bottom;
        return o;
      }

      bool intersects(const rect & r) const
      {
        return
          // first check that both are not empty
          left < right && top < bottom &&
          r.left < r.right && r.top < r.bottom &&
          // now check for intersection
          r.left < right && left < r.right &&
          r.top < bottom && top < r.bottom
          ;
      }

      static bool intersects(const rect & a, const rect & b)
      {
        return a.intersects(b);
      }

      void join(const rect & r);

      bool contains(const rect & r) const;
    };//struct rect

    typedef rect<int32_t> irect;
    */

  }//namespace graphics
}//namespace ds

#endif//__DS_GRAPHICS_RECT__hpp____by_Duzy_Chan__
