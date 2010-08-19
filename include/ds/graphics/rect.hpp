namespace ds { namespace graphics {

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

      void set_empty() { std::memset(this, 0, sizeof(*this)); }
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

      bool intersect(const rect & r)
      {
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

  }//namespace graphics
}//namespace ds
