/**
 *    Copyright 2010-07-25 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ext/slist>
#include <cstring>
#include <ds/graphics/rect.hpp>
#include <ds/graphics/image.hpp>

namespace ds { namespace ui {
    
    struct window::IMPL
    {
      display * _disp; // Never hold a display::pointer_t(will leak)!

      Window _xwin; // Window and Pixmap are Drawable

      XVisualInfo _vi;
      XImage * _ximage;
      ds::graphics::image _image;
      GC _gc;

      __gnu_cxx::slist<ds::graphics::irect> _dirtyRects;

      IMPL( display * d )
        : _disp( d )
        , _xwin( NULL )
        , _vi()
        , _ximage( NULL )
        , _image()
        , _gc( NULL )
        , _dirtyRects()
      {
        std::memset( &_vi, 0, sizeof(_vi) );
      }

      ~IMPL();

      bool get_visual_info( const screen::pointer & scrn );
      bool create_image_if_needed( int w, int h );

      void create( const window::pointer & );
      void destroy();
    };//struct window::IMPL
    
  }//namespace ui
}//namespace ds
