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

#ifdef _WIN32
#    include <windows.h>
typedef HWND    native_window_t;
typedef HDC     native_gc_t;
#elif defined(X11)
#    include <X11/Xlib.h>
#    include <X11/Xutil.h>
typedef Window  native_window_t;
typedef GC      native_gc_t;
#else
#    error unsupported platform
#endif

namespace ds { namespace ui {
    
    struct window::IMPL
    {
      IMPL( const screen::pointer & d );
      ~IMPL();

      bool create_image_if_needed( int w, int h );
      bool create( const display::pointer &, const window::pointer & );
      void destroy( display::IMPL * disp );

      void convert_pixels( int x, int y, int w, int h );

      void select_input(long mask);

      ds::graphics::box get_rect() const;

      ds::graphics::image * get_image_for_render();
      bool commit_image( const ds::graphics::box & dr );

      screen::weak_ref _screen;
      ds::graphics::image _image;
      __gnu_cxx::slist<ds::graphics::box> _dirty_rects;

      native_window_t _native_win;
      native_gc_t _native_gc;

#ifdef _WIN32
#elif defined(X11)
      XImage * _ximage;
      void * _ximage_pixels;

      Display * x_display() const;
#else
#   error unsupported platform
#endif
    };//struct window::IMPL
    
  }//namespace ui
}//namespace ds
