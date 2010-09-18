/**
 *    Copyright 2010-07-25 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/graphics/box.hpp>
#include <ds/graphics/image.hpp>
#include <cstring>
//#include <ext/slist>
#include <list>

#ifdef _WIN32
#    include <windows.h>
#    include "win32/paint_buffer.h"
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
      //typedef __gnu_cxx::slist<ds::graphics::box> box_list_t;
      typedef std::list<ds::graphics::box> box_list_t;

      IMPL( const screen::pointer & d );
      ~IMPL();

      bool create( const display::pointer &, const window::pointer & );
      void destroy( display::IMPL * disp );

      void convert_pixels( int x, int y, int w, int h );

      void select_input(long mask);

      ds::graphics::box get_rect() const;

      void set_dirty( const ds::graphics::box & );
      void redraw_dirties( const display::pointer &, const window::pointer & );
      void pend_update( const ds::graphics::box & );

      bool create_image_if_needed( int w, int h );
      bool commit_updates();

      screen::weak_ref _screen;
      ds::graphics::image _image;

      /**
       *  %_dirty_rects holds a list of rectangles required to be redrawn
       */
      box_list_t _dirty_rects;

      /**
       *  %_pended_updates holds a list of rectangles which has been redrawn
       *  and pending for updating onto the screen.
       */
      box_list_t _pended_updates;

      native_window_t _native_win;
      native_gc_t _native_gc; //!< win32: only availible within BeginPaint/EndPaint

#ifdef _WIN32

      detail::paint_buffer _paint_buffer;

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
