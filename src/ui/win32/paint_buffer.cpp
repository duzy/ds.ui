/**
 *    Copyright 2008-06-15 DuzySoft.com, by Duzy Chan£¨Õ²ÐÀÃú£©
 *    All rights reserved by Duzy Chan
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id: surface.cpp 178 2008-06-25 14:23:36Z duzy $
 *
 **/

/**
 *  This file was modifiled form absent.core/src/plat/win32/surface.cpp.
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <boost/cstdint.hpp>
#include <cstdlib>
#include <cstring> // std::memset
#include "paint_buffer.h"
#include <ds/debug.hpp>

/**

        bool on_draw( const gui::events::draw_event & ev ) {
          int x = 0, y = 0, w = 100, h = 30;
          gui::drawer const & dr = ev.drawer();
          native::drawing_device_handle hdd = dr.handle();

          void * bits = 0;
          BITMAPINFO bi;
          std::memset( &bi, 0, sizeof(BITMAP) );
          bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
          bi.bmiHeader.biWidth = w;
          bi.bmiHeader.biHeight = -h;
          bi.bmiHeader.biPlanes = 1;
          bi.bmiHeader.biBitCount = 32;
          bi.bmiHeader.biCompression = BI_RGB;
          bi.bmiHeader.biSizeImage = w * h * 4;

          enum { F = DIB_RGB_COLORS };

          HDC hdc = ::CreateCompatibleDC( hdd );
          HBITMAP dib = ::CreateDIBSection( hdc, &bi, F, &bits, 0, 0 );
          HGDIOBJ hobjOld = ::SelectObject( hdc, dib );

          typedef typename graphics_base::image_view_t view_t;
          typedef typename graphics_base::pixel_t pixel_t;
          using boost::gil::interleaved_view;
          const view_t v = interleaved_view( w, h, (pixel_t*)bits, w*4 );
          this->render( v, x, y, w, h );

          ::BitBlt( hdd, x, y, w, h, hdc, 0, 0, SRCCOPY );//!< copy to hdd

          ::SelectObject( hdc, hobjOld );
          ::DeleteDC( hdc );
          ::DeleteObject( dib );
        }

 */


namespace ds { namespace ui { namespace detail {

      paint_buffer::paint_buffer()
        : _bmp( NULL )
        , _ptr( NULL )
      {
      }

      paint_buffer::~paint_buffer()
      {
        destroy();
      }

      static inline unsigned
      get_row_size(unsigned width, unsigned bitsPerPixel)
      {
        unsigned n = width;
        unsigned k;

        switch(bitsPerPixel) {
        case  1: k = n;
          n = n >> 3;
          if(k & 7) n++; 
          break;

        case  4: k = n;
          n = n >> 1;
          if(k & 3) n++; 
          break;

        case  8:
          break;

        case 16: n *= 2;
          break;

        case 24: n *= 3; 
          break;

        case 32: n *= 4;
          break;

        case 48: n *= 6; 
          break;

        case 64: n *= 8; 
          break;

        default: n = 0;
          break;
        }
        return ((n + 3) >> 2) << 2;
      }

      static inline unsigned
      get_palette_size(unsigned colorUsed, unsigned bitsPerPixel)
      {
        int paletteSize = 0;

        if(bitsPerPixel <= 8) {
          paletteSize = colorUsed;
          if(paletteSize == 0) {
            paletteSize = 1 << bitsPerPixel;
          }
        }

        return paletteSize;
      }

      void paint_buffer::create( unsigned w, unsigned h, unsigned bitsPerPixel )
      {
        unsigned rowSize = get_row_size( w, bitsPerPixel );
        unsigned imageSize = rowSize * h;
        unsigned rgbSize = get_palette_size( 0, bitsPerPixel ) * sizeof(RGBQUAD);
        unsigned fullSize = sizeof(BITMAPINFOHEADER) + rgbSize + imageSize;
        
        _bmp = (BITMAPINFO*) new uint8_t[fullSize];
        _bmp->bmiHeader.biSize   = sizeof(BITMAPINFOHEADER);
        _bmp->bmiHeader.biWidth  = w;
        _bmp->bmiHeader.biHeight = -h; // up-side-down
        _bmp->bmiHeader.biPlanes = 1;
        _bmp->bmiHeader.biBitCount = (unsigned short)bitsPerPixel;
        _bmp->bmiHeader.biCompression = BI_RGB;
        _bmp->bmiHeader.biSizeImage = imageSize;
        _bmp->bmiHeader.biXPelsPerMeter = 0;
        _bmp->bmiHeader.biYPelsPerMeter = 0;
        _bmp->bmiHeader.biClrUsed = 0;
        _bmp->bmiHeader.biClrImportant = 0;

        _ptr = ((uint8_t*)_bmp) + sizeof(BITMAPINFOHEADER) + rgbSize;
      }

      void paint_buffer::destroy()
      {
        delete [] (uint8_t*) _bmp;

        _bmp = NULL;
        _ptr = NULL;
      }

      bool paint_buffer::flush(HDC dc, const RECT *dstRect, const RECT *srcRect) const
      {
        if (_bmp == 0 || _ptr == 0) {
          dsE("paint buffer is null");
          return false;
        }

        unsigned srcX = 0;
        unsigned srcY = 0;
        unsigned srcW = _bmp->bmiHeader.biWidth;
        unsigned srcH = _bmp->bmiHeader.biHeight;
        unsigned dstX = 0;
        unsigned dstY = 0; 
        unsigned dstW = _bmp->bmiHeader.biWidth;
        unsigned dstH = _bmp->bmiHeader.biHeight;
        
        if (srcRect) {
          srcX = srcRect->left;
          srcY = srcRect->top;
          srcW = srcRect->right  - srcRect->left;
          srcH = srcRect->bottom - srcRect->top;
        }

        dstX = srcX;
        dstY = srcY;
        dstW = srcW;
        dstH = srcH;

        if (dstRect) {
          dstX = dstRect->left;
          dstY = dstRect->top;
          dstW = dstRect->right  - dstRect->left;
          dstH = dstRect->bottom - dstRect->top;
        }

        int n = 0;
        if (dstW != srcW || dstH != srcH) {
          ::SetStretchBltMode(dc, COLORONCOLOR);
          n = ::StretchDIBits
            ( dc,      // handle of device context 
              dstX,    // x-coordinate of upper-left corner of source rect. 
              dstY,    // y-coordinate of upper-left corner of source rect. 
              dstW,    // width of source rectangle 
              dstH,    // height of source rectangle 
              srcX,
              srcY,    // x, y -coordinates of upper-left corner of dest. rect. 
              srcW,    // width of destination rectangle 
              srcH,    // height of destination rectangle 
              _ptr,    // address of bitmap bits 
              _bmp,    // address of bitmap data 
              DIB_RGB_COLORS,  // usage 
              SRCCOPY          // raster operation code 
              );
        }
        else {
          n = ::SetDIBitsToDevice
            ( dc,       // handle to device context
              dstX,     // x-coordinate of upper-left corner of 
              dstY,     // y-coordinate of upper-left corner of 
              dstW,     // source rectangle width
              dstH,     // source rectangle height
              srcX,     // x-coordinate of lower-left corner of 
              srcY,     // y-coordinate of lower-left corner of 
              0,        // first scan line in array
              srcH,     // number of scan lines
              _ptr,     // address of array with DIB bits
              _bmp,     // address of structure with bitmap info.
              DIB_RGB_COLORS   // RGB or palette indexes
              );
        }
        return (n != 0);
      }
    
    }//namespace detail
  }//namespace ui
}//namespace ds
