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

        void flush(HDC dc, const RECT *dst, const RECT *src) const;

        uint8_t * buffer() const { return _buffer; }

        std::size_t width() const
        {
          if ( _bi == NULL ) return 0;
          return _bi->bmiHeader.biWidth; //!< NOTICE: signed to unsigned
        }

        std::size_t height() const
        {
          if ( _bi == NULL ) return 0;
          return std::abs(_bi->bmiHeader.biHeight);
        }

        std::size_t stride() const //!< bytes per line
        {
          if ( _bi == NULL ) return 0;
          return _bi->bmiHeader.biWidth * _bi->bmiHeader.biBitCount;
        }

        BITMAPINFO        *_bi;
        uint8_t           *_buffer;
      };//struct paint_buffer

    }//namespace detail
  }//namespace ui
}//namespace ds

#endif//_DS_UI_DETAIL_PAINT_BUFFER_
