/**
 *    Copyright 2010-07-25 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/ui/window.hpp>
#include <ds/ui/display.hpp>
#include <X11/Xlib.h>
#include "window_impl.h"
#include "display_impl.h"

namespace ds { namespace ui {
    
    window::window()
      : _p( new IMPL(NULL) )
    {
    }

    window::window( display * disp )
      : _p( new IMPL(disp) )
    {
      //XCreateWindow(...);
      Display *xDisplay( disp->_p->xDisplay );
      int x(0), y(0), w(400), h(300), bw(0);
      unsigned fc = disp->black_pixel();
      unsigned bc = disp->white_pixel();

      Window pr = disp->root()->_p->xWindow;
      
      _p->xWindow = XCreateSimpleWindow( xDisplay, pr, x, y, w, h, bw, fc, bc );
      disp->add( this );
    }

    display * window::get_display() const
    {
      return _p->disp;
    }

    void window::show()
    {
      
    }

    void window::hide()
    {
    }

    void window::close()
    {
    }

    void window::move( int x, int y )
    {
    }

    void window::resize( int w, int h )
    {
    }
    
  }// namespace ui
}//namespace ds
