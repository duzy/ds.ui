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
#include <ds/graphics/box.hpp>
#include <ds/graphics/image.hpp>

namespace ds { namespace ui {
    
    struct window::IMPL
    {
      screen::weak_ref _screen;

      Window _xwin; // Window and Pixmap are Drawable

      XVisualInfo _vi;
      XImage * _ximage;
      void * _ximage_pixels;
      ds::graphics::image _image;
      GC _gc;

      __gnu_cxx::slist<ds::graphics::box> _dirtyRects;

      IMPL( const screen::pointer & d )
        : _screen( d )
        , _xwin( NULL )
        , _vi()
        , _ximage( NULL )
        , _ximage_pixels( NULL )
        , _image()
        , _gc( NULL )
        , _dirtyRects()
      {
        std::memset( &_vi, 0, sizeof(_vi) );
      }

      ~IMPL();

      Display * x_display() const;

      bool get_visual_info( const screen::pointer & scrn );
      bool create_image_if_needed( int w, int h );

      void create( const window::pointer & );
      void destroy();

      void convert_pixels( int x, int y, int w, int h ); 
    };//struct window::IMPL
    
  }//namespace ui
}//namespace ds
