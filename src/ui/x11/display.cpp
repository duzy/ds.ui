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
#include <ds/ui/events.hpp>
#include <ds/event_queue.hpp>
#include <ds/debug.hpp>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <sys/types.h>
#include "display_impl.h"
#include "screen_impl.h"
#include "window_impl.h"

namespace ds { namespace ui {

    void display::IMPL::open( const display::pointer & disp, const char * name )
    {
      _xdisplay = XOpenDisplay( name );

      int scrnCount( XScreenCount( _xdisplay ) );
      dsI( 0 < scrnCount );

      _scrns = new screen::pointer [scrnCount];
      for ( int n = 0; n < scrnCount; ++n ) {
        screen::pointer s( new screen );
        s->_p->_xscrn = XScreenOfDisplay( _xdisplay, n );
        s->_p->_display = disp;
        _scrns[n] = s;
      }

      _winmap.clear();

      init_atoms();
    }

    void display::IMPL::init_atoms()
    {
#define GET_ATOM(X) X = XInternAtom( _xdisplay, #X, False )

      GET_ATOM(WM_DELETE_WINDOW);

#undef GET_ATOM
    }

    bool display::IMPL::pending()
    {
      XFlush(_xdisplay);
      if (XEventsQueued(_xdisplay, QueuedAlready)) {
        return true;
      }

      /* More drastic measures are required -- see if X is ready to talk */
      {
        static struct timeval zeroTv;        /* static == 0 */
        int fd;
        fd_set fdset;
        
        fd = XConnectionNumber(_xdisplay);
        FD_ZERO(&fdset);
        FD_SET(fd, &fdset);
        if (select(fd + 1, &fdset, NULL, NULL, &zeroTv) == 1) {
          return (XPending(_xdisplay));
        }
      }

      return false;
    }

    void display::IMPL::pump_events( event_queue * eq )
    {
      // see SDL/src/video/x11/SDL_x11events.c
      if ( eq == NULL ) {
        dsE("event_queue unset, should call display::start first");
        return;
      }
      if ( _xdisplay == NULL ) return;

      XEvent event;
      //bzero( &event, sizeof(event) );

      while ( pending() ) {
        XNextEvent( _xdisplay, &event );
        push_event( eq, &event );
      }
    }

    void display::IMPL::map( const window::pointer & win )
    {
      window::IMPL * p = win->_p;

      dsI( p );

      if ( /*!p->_disp ||*/ !p->_xwin ) {

        const int sn = XDefaultScreen( _xdisplay );
        const int sc = XScreenCount( _xdisplay );

        dsI( 0 <= sn && sn < sc );
        dsI( _scrns != NULL );
        
        p->_screen = _scrns[ sn ];
        p->create( win );
      }

      XMapWindow( _xdisplay, p->_xwin );
    }

    void display::IMPL::unmap( const window::pointer & win )
    {
      window::IMPL * p = win->_p;

      dsI( p );
      dsI( _xdisplay );

      if ( p->_xwin ) {
        /**
         *  This will send UnmapNotify, and it's handler will erase win from
         *  _winmap .
         */
        XUnmapWindow( _xdisplay, p->_xwin );
      }
    }

    bool display::IMPL::has( const window::pointer & win )
    {
      if (!win->_p->_xwin) return false;
      return (_winmap.find( win->_p->_xwin ) != _winmap.end());
    }

    Window display::IMPL::default_root() const
    {
      return XDefaultRootWindow( _xdisplay );
    }

    bool display::IMPL::is_default_root( const window::pointer & w ) const
    {
      return w->_p->_xwin == XDefaultRootWindow( _xdisplay );
    }
  }//namespace ui
}//namespace ds
