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
#include <sys/types.h>
#include "display_impl.h"
#include "screen_impl.h"
#include "window_impl.h"
#include <ds/debug.hpp>

namespace ds { namespace ui {

    void display::IMPL::init_atoms()
    {
#define GET_ATOM(X) X = XInternAtom( xDisplay, #X, False )

      GET_ATOM(WM_DELETE_WINDOW);

#undef GET_ATOM
    }

    bool display::IMPL::pending()
    {
      XFlush(xDisplay);
      if (XEventsQueued(xDisplay, QueuedAlready)) {
        return true;
      }

      /* More drastic measures are required -- see if X is ready to talk */
      {
        static struct timeval zeroTv;        /* static == 0 */
        int fd;
        fd_set fdset;
        
        fd = XConnectionNumber(xDisplay);
        FD_ZERO(&fdset);
        FD_SET(fd, &fdset);
        if (select(fd + 1, &fdset, NULL, NULL, &zeroTv) == 1) {
          return (XPending(xDisplay));
        }
      }

      return false;
    }

    void display::IMPL::pump_events()
    {
      // see SDL/src/video/x11/SDL_x11events.c

      if ( xDisplay == NULL ) return;
      //if ( screen == -1 ) return;

      XEvent event;
      //bzero( &event, sizeof(event) );

      while ( true )
      while ( pending() ) {
        XNextEvent( xDisplay, &event );
        dispatch( &event );
      }
    }

    void display::IMPL::dispatch( XEvent * event )
    {
      //std::cout << "event: " << event->type << std::endl;

      /* filter events catchs XIM events and sends them to the correct
         handler */
      if ( XFilterEvent(event, None) == True ) {
        return;
      }

      switch (event->type) {
      case ClientMessage:
        if (event->xclient.format == 32 &&
            event->xclient.data.l[0] == WM_DELETE_WINDOW) {
          std::cout<<"WM_DELETE_WINDOW"<<std::endl;
          //Window w = event->xclient->window;
          Window w = event->xany.window;
          // TODO: destroy window
        }
        break;
      }

      //TODO: send events to my event_queue
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
      //d->_p->screen = XDefaultScreen( d->_p->xDisplay );
      d->_p->init_atoms();
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
      //XSync( _p->xDisplay, False );

      _p->pump_events();

      return 0;
    }
    
  }//namespace ui
}//namespace ds
