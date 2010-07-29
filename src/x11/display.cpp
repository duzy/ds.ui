/**
 *    Copyright 2010-07-25 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/
#include <ds/ui/display.hpp>
#include <ds/ui/screen.hpp>
#include <ds/ui/window.hpp>
#include <X11/Xlib.h>
#include "display_impl.h"
#include "screen_impl.h"
#include "window_impl.h"
#include <ds/debug.hpp>

namespace ds { namespace ui {

    void display::IMPL::pump_events()
    {
      if ( xDisplay == NULL ) return;
      if ( screen == -1 ) return;

      XEvent event;
      //bzero( &event, sizeof(event) );

      //while ( XPending(xDisplay) ) {
      while ( true ) {
        XNextEvent( xDisplay, &event );
        dispatch( &event );
      }
    }

    void display::IMPL::dispatch( XEvent * event )
    {
      // TODO: ...
      std::cout << "event: " << event->type << std::endl;
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
      //dsD("display::~display()");
      std::cout<<"display::~display()"<<std::endl;

      if ( _p->xDisplay )
        XCloseDisplay( _p->xDisplay );
      delete _p;
    }

    display::pointer_t display::open( id i )
    {
      pointer_t d( new display );
      d->_p->xDisplay = XOpenDisplay( (const char *) i._p );
      d->_p->screen = XDefaultScreen( d->_p->xDisplay );
      return d;
    }

    screen::pointer_t display::default_screen() const
    {
      screen::pointer_t scr( new screen );
      scr->_p->xScreen = XDefaultScreenOfDisplay( _p->xDisplay );
      return scr;
    }

    screen::pointer_t display::get_screen( int index ) const
    {
      screen::pointer_t scr( new screen );
      scr->_p->xScreen = XScreenOfDisplay( _p->xDisplay, index );
      return scr;
    }

    window::pointer_t display::default_root() const
    {
      window::pointer_t w( new window );
      w->_p->disp = const_cast<display*>(this);
      w->_p->xWindow = XDefaultRootWindow( _p->xDisplay );
      return w;
    }
    
    void display::map( const window::pointer_t & win )
    {
      if ( /*!win->_p->disp ||*/ !win->_p->xWindow ) {
        win->_p->disp = this; // convert to display::pointer_t
        win->_p->create();
      }

      XMapWindow( _p->xDisplay, win->_p->xWindow );
    }

    void display::unmap( const window::pointer_t & win )
    {
      XUnmapWindow( _p->xDisplay, win->_p->xWindow );
    }

    bool display::has( const window::pointer_t & win )
    {
      // TODO: ...
      //return false;
      return true;
    }

    int display::reduce_events()
    {
      XSync( _p->xDisplay, False );

      _p->pump_events();

      return 0;
    }
    
  }//namespace ui
}//namespace ds
