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
#include <ds/ui/screen.hpp>
#include <X11/Xlib.h>
#include "window_impl.h"
#include "display_impl.h"
#include <ds/debug.hpp>

namespace ds { namespace ui {

    void window::IMPL::create()
    {
      dsI( !xWindow );
      dsI( disp );

      int x(0), y(0), w(400), h(300), bw(0);

      screen::pointer_t scrn = disp->default_screen();
      unsigned fc = scrn->black_pixel();
      unsigned bc = scrn->white_pixel();

      window::pointer_t root = scrn->root();
      dsI(root);

      Window pr = root->_p->xWindow;
      register Display *xDisplay( disp->_p->xDisplay );

      //XCreateWindow(...);
      xWindow = XCreateSimpleWindow( xDisplay, pr, x, y, w, h, bw, fc, bc );

      /* Allow window to be deleted by the window manager */
      XSetWMProtocols( xDisplay, xWindow, &disp->_p->WM_DELETE_WINDOW, 1 );

      int eventMask
        = KeyPressMask
        | KeyReleaseMask
        | ButtonPressMask
        | ButtonReleaseMask
        | EnterWindowMask
        | LeaveWindowMask
        | PointerMotionMask
        //| PointerMotionHintMask
        | Button1MotionMask
        | Button2MotionMask
        | Button3MotionMask
        | Button4MotionMask
        | ButtonMotionMask
        //| KeymapStateMask
        | ExposureMask
        | VisibilityChangeMask
        | StructureNotifyMask
        | ResizeRedirectMask
        | SubstructureNotifyMask
        | SubstructureRedirectMask
        | FocusChangeMask
        | PropertyChangeMask
        | ColormapChangeMask
        | OwnerGrabButtonMask
        ;
      XSelectInput( xDisplay, xWindow, eventMask );
    }

    void window::IMPL::destroy()
    {
      XDestroyWindow( disp->_p->xDisplay, xWindow );
    }

    //////////////////////////////////////////////////////////////////////
    
    window::window()
      : _p( new IMPL(NULL) )
    {
    }

    window::window( const display::pointer_t & disp )
      : _p( new IMPL(disp) )
    {
      _p->create();
      disp->map( this );
    }

    window::~window()
    {
      dsD("window::~window: "<<this);

      _p->destroy();
      _p->disp.reset( NULL );
      delete _p;
    }

    display::pointer_t window::get_display() const
    {
      return _p->disp;
    }

    void window::select_input(long mask)
    {
      XSelectInput( _p->disp->_p->xDisplay, _p->xWindow, mask );
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
