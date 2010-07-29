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
#include <X11/Xlib.h>
#include <sys/types.h>
#include "display_impl.h"
#include "screen_impl.h"
#include "window_impl.h"
#include <ds/debug.hpp>

#include "display_events.ipp"

namespace ds { namespace ui {

    void display::IMPL::init( display *disp, const char * name )
    {
      _xdisp = XOpenDisplay( name );

      int scrnCount( XScreenCount( _xdisp ) );
      dsI( 0 < scrnCount );

      _scrns = new screen::pointer_t [scrnCount];
      for ( int n = 0; n < scrnCount; ++n ) {
        screen::pointer_t s( new screen );
        s->_p->_xscrn = XScreenOfDisplay( _xdisp, n );
        s->_p->_disp = disp;
        _scrns[n] = s;
      }

      _winmap.clear();

      init_atoms();
    }

    void display::IMPL::init_atoms()
    {
#define GET_ATOM(X) X = XInternAtom( _xdisp, #X, False )

      GET_ATOM(WM_DELETE_WINDOW);

#undef GET_ATOM
    }

    bool display::IMPL::pending()
    {
      XFlush(_xdisp);
      if (XEventsQueued(_xdisp, QueuedAlready)) {
        return true;
      }

      /* More drastic measures are required -- see if X is ready to talk */
      {
        static struct timeval zeroTv;        /* static == 0 */
        int fd;
        fd_set fdset;
        
        fd = XConnectionNumber(_xdisp);
        FD_ZERO(&fdset);
        FD_SET(fd, &fdset);
        if (select(fd + 1, &fdset, NULL, NULL, &zeroTv) == 1) {
          return (XPending(_xdisp));
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
      if ( _xdisp == NULL ) return;

      XEvent event;
      //bzero( &event, sizeof(event) );

      while ( pending() ) {
        XNextEvent( _xdisp, &event );
        push_event( eq, &event );
      }
    }

    //////////////////////////////////////////////////////////////////////

    display::id::id( void *p ) : _p( p ) {}

    //////////////////////////////////////////////////////////////////////

    display::display()
      : event_pump( NULL )
      , _p( new IMPL )
    {
      dsL("display: "<<this<<"->"<<_p->_xdisp);
    }

    display::~display()
    {
      dsL("display: "<<this<<"->"<<_p->_xdisp);

      if ( _p->_xdisp ) {
        XCloseDisplay( _p->_xdisp );
        _p->_winmap.clear();
      }

      delete [] _p->_scrns;
      delete _p;
    }

    display::pointer_t display::open( id i )
    {
      pointer_t d( new display );
      d->_p->init( d.get(), (const char *) i._p );
      return d;
    }

    screen::pointer_t display::get_screen( int n ) const
    {
      dsI( 0 <= n && n < screen_count() );
      dsI( _p->_scrns );
      return _p->_scrns[n];
    }

    int display::default_screen_number() const
    {
      return XDefaultScreen( _p->_xdisp );
    }

    int display::screen_count() const
    {
      return XScreenCount( _p->_xdisp );
    }

    window::pointer_t display::default_root() const
    {
      window::pointer_t w( new window ); // TODO: avoid making a new instance of window
      w->_p->_disp = const_cast<display*>(this);
      w->_p->_xwin = XDefaultRootWindow( _p->_xdisp );
      return w;
    }
    
    void display::map( const window::pointer_t & win )
    {
      if ( /*!win->_p->_disp ||*/ !win->_p->_xwin ) {
        win->_p->_disp = this; // implicitly convert to display::pointer_t
        win->_p->create( win );
      }

      XMapWindow( _p->_xdisp, win->_p->_xwin );
    }

    void display::unmap( const window::pointer_t & win )
    {
      XUnmapWindow( _p->_xdisp, win->_p->_xwin );
    }

    bool display::has( const window::pointer_t & win )
    {
      // TODO: ...
      //return false;
      return true;
    }

    void display::pump_events()
    {
      _p->pump_events( event_pump::get_queue() );
    }
    
  }//namespace ui
}//namespace ds
