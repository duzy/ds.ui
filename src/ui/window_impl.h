/**
 *    Copyright 2010-07-25 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#  if defined(QT)
#    include <QtGui/QApplication>
#    include <QtGui/QWidget>
typedef QWidget*  native_window_t;
#elif defined(_WIN32)
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
#include <ds/graphics/region.hpp>
#include <ds/graphics/image.hpp>
#include <cstring>

namespace ds { namespace ui {

    struct window::IMPL
    {
      IMPL( const screen::pointer & d );
      ~IMPL();

      bool create_natively( const display::pointer &, const window::pointer & );
      void destroy_natively( display::IMPL * disp );

      void show_natively();
      void hide_natively();

      void select_input(long mask);

      ds::graphics::box get_rect() const;
      ds::graphics::box get_client_rect() const;

      void set_dirty( const ds::graphics::box & );
      void redraw_dirties( const display::pointer &, const window::pointer & );
      void pend_update( const ds::graphics::box & );

      bool create_image_if_needed( int w, int h );
      bool commit_update_natively( const ds::graphics::box & b );
      bool sync_updates_natively();
      bool commit_updates();

      screen::weak_ref _screen;
      ds::graphics::image _image;

      /**
       *  %_dirty_rects holds a list of rectangles required to be redrawn
       */
      ds::graphics::region _dirty_region; //!< the first one is the bounding

      /**
       *  %_pended_updates holds a list of rectangles which has been redrawn
       *  and pending for updating onto the screen.
       */
      ds::graphics::region _pended_updates; //!< the first one is the bounding

      native_window_t _native_win;

#  if defined(QT)

      //QImage _qimage;

#elif defined(_WIN32)

      native_gc_t _native_gc; //!< win32: only availible within BeginPaint/EndPaint
      detail::paint_buffer _paint_buffer;

#elif defined(X11)

      native_gc_t _native_gc;
      XImage * _ximage;
      //void * _ximage_pixels;

      Display * x_display() const;

#else
#   error unsupported platform
#endif
    };//struct window::IMPL

  }//namespace ui
}//namespace ds
