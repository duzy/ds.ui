/**
 *    Copyright 2010-07-25 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/
#include <ds/ui/display.hpp>
#include <ds/ui/window.hpp>
#include <X11/Xlib.h>
#include "display_impl.h"
#include "window_impl.h"
#include <ds/debug.hpp>

namespace ds { namespace ui {

    void display::IMPL::loop()
    {
      if ( xDisplay == NULL ) return;
      if ( screen == -1 ) return;

      XEvent ev;
      while ( true ) {
        XNextEvent( xDisplay, &ev );
        dispatch( &ev );
      }
    }

    void display::IMPL::dispatch( XEvent * event )
    {
      // TODO: ...
    }

    //////////////////////////////////////////////////////////////////////

    display::id::id( void *p ) : _p( p ) {}

    //////////////////////////////////////////////////////////////////////

    display::display()
      : _p( new IMPL )
    {
    }

    display::~display()
    {
    }

    display::pointer_t display::open( id i )
    {
      pointer_t d( new display );
      d->_p->xDisplay = XOpenDisplay( (const char *) i._p );
      d->_p->screen = XDefaultScreen( d->_p->xDisplay );
      return d;
    }

    int display::width() const
    {
      return XDisplayWidth( _p->xDisplay, _p->screen );
    }

    int display::height() const
    {
      return XDisplayHeight( _p->xDisplay, _p->screen );
    }

    window *display::root() const
    {
      if ( _p->root )
        return _p->root;

      Window xWindow = XRootWindow( _p->xDisplay, _p->screen );
      _p->root = new window;
      _p->root->_p->disp = const_cast<display*>(this);
      _p->root->_p->xWindow = xWindow;
      
      return _p->root;
    }
    
    void display::add( window * win )
    {
      XMapWindow( _p->xDisplay, win->_p->xWindow );
    }

    void display::remove( window * win )
    {
      XUnmapWindow( _p->xDisplay, win->_p->xWindow );
    }

    bool display::has( window * win )
    {
      // TODO: ...
      //return false;
      return true;
    }

    unsigned display::black_pixel() const
    {
      return XBlackPixel( _p->xDisplay, _p->screen );
    }

    unsigned display::white_pixel() const
    {
      return XWhitePixel( _p->xDisplay, _p->screen );
    }

    int display::reduce_events( window * win )
    {
      XSync( _p->xDisplay, False );

      int eventMask = ExposureMask
        | ButtonPressMask
        | PointerMotionMask
        ;
      XSelectInput( _p->xDisplay, win->_p->xWindow, eventMask );

      _p->loop();

      XCloseDisplay( _p->xDisplay );

      return 0;
    }
    
  }//namespace ui
}//namespace ds
