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

    void window::IMPL::create( const window::pointer & win )
    {
      dsI( !_xwin );
      dsI( _disp );

      int x(0), y(0), w(400), h(300), bw(0);

      screen::pointer scrn = _disp->default_screen();
      unsigned fc = scrn->black_pixel();
      unsigned bc = scrn->white_pixel();

      window::pointer root = scrn->root();
      dsI(root);

      Window pr = root->_p->_xwin;
      register Display *xDisplay( _disp->_p->_xdisp );

      //XCreateWindow(...);
      _xwin = XCreateSimpleWindow( xDisplay, pr, x, y, w, h, bw, fc, bc );

      _disp->_p->_winmap.insert( std::make_pair( _xwin, win ) );

      /* Allow window to be deleted by the window manager */
      XSetWMProtocols( xDisplay, _xwin, &_disp->_p->WM_DELETE_WINDOW, 1 );

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
      XSelectInput( xDisplay, _xwin, eventMask );
    }

    void window::IMPL::destroy()
    {
      if ( _xwin ) {
        XDestroyWindow( _disp->_p->_xdisp, _xwin );
      }
    }

    //////////////////////////////////////////////////////////////////////
    
    window::window()
      : _p( new IMPL(NULL) )
    {
      dsL4("window: "<<this<<"->"<<_p->_xwin);
    }

    window::window( const display::pointer & disp )
      : _p( new IMPL(disp.get()) )
    {
      _p->create( this );
      disp->map( this );
      dsL4("window: "<<this<<"->"<<_p->_xwin);
    }

    window::~window()
    {
      dsL4("window: "<<this<<"->"<<_p->_xwin);

      _p->destroy();
      _p->_disp = NULL;
      delete _p;
    }

    display::pointer window::get_display() const
    {
      return _p->_disp;
    }

    void window::select_input(long mask)
    {
      XSelectInput( _p->_disp->_p->_xdisp, _p->_xwin, mask );
    }

    void window::destroy()
    {
      _p->destroy();
    }

    void window::show()
    {
      
    }

    void window::hide()
    {
    }

    void window::close()
    {
      /**
       *  1): send close event (to perform on_close)
       *  2): perform close
       */
      event::window::close evt;
      evt.win = this;
      this->on_close( evt );
    }

    void window::move( int x, int y )
    {
    }

    void window::resize( int w, int h )
    {
    }

    void window::on_shown( const event::window::shown & a )
    {
      dsL("winact: "<<a.win);
    }

    void window::on_hidden( const event::window::hidden & a )
    {
      dsL("winact: "<<a.win);
    }

    void window::on_exposed( const event::window::exposed & a )
    {
      dsL("winact: "<<a.win);
    }

    void window::on_moved( const event::window::moved & a )
    {
      dsL("winact: "<<a.win);
    }

    void window::on_resized( const event::window::resized & a )
    {
      dsL("winact: "<<a.win);
    }

    void window::on_minimized( const event::window::minimized & a )
    {
      dsL("winact: "<<a.win);
    }

    void window::on_maximized( const event::window::maximized & a )
    {
      dsL("winact: "<<a.win);
    }

    void window::on_restored( const event::window::restored & a )
    {
      dsL("winact: "<<a.win);
    }

    void window::on_enter( const event::window::enter & a )
    {
      dsL("winact: "<<a.win);
    }

    void window::on_leave( const event::window::leave & a )
    {
      dsL("winact: "<<a.win);
    }

    void window::on_focus( const event::window::focus & a )
    {
      dsL("winact: "<<a.win);
    }

    void window::on_close( const event::window::close & a )
    {
      dsL("winact: "<<a.win);
      do_close(); // Don't call 'close()'!
    }

    void window::do_close()
    {
      destroy(); // TODO: should be destroyed?
    }
    
  }// namespace ui
}//namespace ds
