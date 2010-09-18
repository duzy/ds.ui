/**
 *    Copyright 2010-09-17 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#ifndef _DS_UI_DETAIL_PAINT_BUFFER_
#define _DS_UI_DETAIL_PAINT_BUFFER_ 1

namespace ds { namespace ui { namespace detail {

      /**
       *  For WM_PAINT
       */
      struct paint_buffer
      {
        paint_buffer();
        ~paint_buffer();

        void create( unsigned w, unsigned h, unsigned bitsPerPixel );
        void destroy();

        bool flush(HDC dc, const RECT *dst, const RECT *src) const;

        BITMAPINFO * bitmap_info() const { return _bmp; }
        uint8_t * ptr() const { return _ptr; }

        std::size_t width() const
        {
          if ( _bmp == NULL ) return 0;
          return _bmp->bmiHeader.biWidth; //!< NOTICE: signed to unsigned
        }

        std::size_t height() const
        {
          if ( _bmp == NULL ) return 0;
          return std::abs(_bmp->bmiHeader.biHeight);
        }

        std::size_t stride() const //!< bytes per line
        {
          if ( _bmp == NULL ) return 0;
          return _bmp->bmiHeader.biWidth * _bmp->bmiHeader.biBitCount;
        }

      private:
        BITMAPINFO        *_bmp;
        uint8_t           *_ptr;
      };//struct paint_buffer

    }//namespace detail
  }//namespace ui
}//namespace ds

#endif//_DS_UI_DETAIL_PAINT_BUFFER_
