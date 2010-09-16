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
#include "../window_impl.h"
#include "../screen_impl.h"
#include "../display_impl.h"

namespace ds { namespace ui {

    display::IMPL::IMPL()
      : _xdisplay( NULL )
      , _scrns( NULL )
      , _winmap()
      , WM_DELETE_WINDOW( 0 )
    {
    }

    display::IMPL::~IMPL()
    {
      dsI( _xdisplay );

      //!< destroy all mapped windows, all window::pointer references will
      //!< be invalid
      window_map_t::iterator it = _winmap.begin();
      for(; it != _winmap.end(); ++it) {
        it->second->_p->destroy( _xdisplay );
      }
      _winmap.clear();

      XCloseDisplay( _xdisplay );

      delete [] _scrns;
    }

    void display::IMPL::init_atoms()
    {
#define GET_ATOM(X) X = XInternAtom( _xdisplay, #X, False )

      GET_ATOM(WM_DELETE_WINDOW);

#undef GET_ATOM
    }

    void display::IMPL::open( const display::pointer & disp, const char * name )
    {
      dsI( !_xdisplay );
      dsI( !_scrns );
      dsI( _winmap.empty() );

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

    void display::IMPL::pump_native_events( event_queue * eq )
    {
      // see SDL/src/video/x11/SDL_x11events.c
      if ( eq == NULL ) {
        dsE("event_queue unset, should call display::start first");
        return;
      }
      if ( _xdisplay == NULL ) return;

      XEvent event;

      //bzero( &event, sizeof(event) );
      memset( &event, 0, sizeof(event) );

      while ( pending() ) {
        XNextEvent( _xdisplay, &event );
        push_event( eq, &event );
      }
    }

    bool display::IMPL::map_win_natively( const window::pointer & win )
    {
      window::IMPL * p = win->_p;

      dsI( p );
      dsI( p->_native_win );

      // TODO: check if the window has been mapped in this display

      XMapWindow( _xdisplay, p->_native_win );
      return true;
    }

    bool display::IMPL::unmap_win_natively( const window::pointer & win )
    {
      window::IMPL * p = win->_p;

      dsI( p );
      dsI( _xdisplay );

      if ( p->_native_win ) {
        /**
         *  This will send UnmapNotify, and it's handler will erase win from
         *  _winmap .
         */
        XUnmapWindow( _xdisplay, p->_native_win );
        return true;
      }
      return false;
    }

    bool display::IMPL::is_win_mapped_natively( const window::pointer & win )
    {
      return (win->_p->_native_win);
    }

    Window display::IMPL::default_root() const
    {
      return XDefaultRootWindow( _xdisplay );
    }

    bool display::IMPL::is_default_root( const window::pointer & w ) const
    {
      return w->_p->_native_win == XDefaultRootWindow( _xdisplay );
    }

    int display::IMPL::screen_count() const
    {
      dsI( _xdisplay );
      return XScreenCount( _xdisplay );
    }

    int display::IMPL::default_screen_number() const
    {
      dsI( _xdisplay );
      return XDefaultScreen( _xdisplay );
    }

    screen::pointer display::IMPL::get_screen( int n ) const
    {
      if ( 0 <= n && n < screen_count() ) {
        dsI( _scrns );
        return _scrns[n];
      }
      return NULL;
    }
  }//namespace ui
}//namespace ds
