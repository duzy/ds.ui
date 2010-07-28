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
      //std::cout << "event: " << event->type << std::endl;
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
      delete _p;
      //dsD("display::~display()");
      std::cout<<"display::~display()"<<std::endl;
    }

    display::pointer_t display::open( id i )
    {
      pointer_t d( new display );
      d->_p->xDisplay = XOpenDisplay( (const char *) i._p );
      d->_p->screen = XDefaultScreen( d->_p->xDisplay );
      return d;
    }

    shared_object<screen>::pointer_t display::default_screen() const
    {
      screen::pointer_t scr( new screen );
      scr->_p->xScreen = XDefaultScreenOfDisplay( _p->xDisplay );
      return scr;
    }

    shared_object<screen>::pointer_t display::get_screen( int index ) const
    {
      screen::pointer_t scr( new screen );
      scr->_p->xScreen = XScreenOfDisplay( _p->xDisplay, index );
      return scr;
    }

    shared_object<window>::pointer_t display::root() const
    {
      if ( _p->root )
        return _p->root;

      Window xWindow = XRootWindow( _p->xDisplay, _p->screen );
      _p->root = new window;
      _p->root->_p->disp = const_cast<display*>(this);
      _p->root->_p->xWindow = xWindow;
      
      return _p->root;
    }
    
    void display::add( const window::pointer_t & win )
    {
      XMapWindow( _p->xDisplay, win->_p->xWindow );
    }

    void display::remove( const window::pointer_t & win )
    {
      XUnmapWindow( _p->xDisplay, win->_p->xWindow );
    }

    bool display::has( const window::pointer_t & win )
    {
      // TODO: ...
      //return false;
      return true;
    }

    //int display::reduce_events( const window::pointer_t & win )
    int display::reduce_events()
    {
      XSync( _p->xDisplay, False );

      _p->loop();

      XCloseDisplay( _p->xDisplay );

      return 0;
    }
    
  }//namespace ui
}//namespace ds
