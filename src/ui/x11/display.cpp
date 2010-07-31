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
#include <X11/Xutil.h>
#include <sys/types.h>
#include "display_impl.h"
#include "screen_impl.h"
#include "window_impl.h"
#include <ds/debug.hpp>

namespace ds { namespace ui {

    void display::IMPL::init( const display::pointer & disp, const char * name )
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

    //////////////////////////////////////////////////////////////////////

    display::id::id( void *p ) : _p( p ) {}

    //////////////////////////////////////////////////////////////////////

    display::display()
      : event_pump( NULL )
      , _p( new IMPL )
    {
      dsL("display: "<<this<<"->"<<_p->_xdisplay);
    }

    display::~display()
    {
      dsL("display: "<<this<<"->"<<_p->_xdisplay);

      if ( _p->_xdisplay ) {
        XCloseDisplay( _p->_xdisplay );
        _p->_winmap.clear();
      }

      delete [] _p->_scrns;
      delete _p;
    }

    display::pointer display::open( id i )
    {
      pointer d( new display );
      d->_p->init( d, (const char *) i._p );
      return d;
    }

    screen::pointer display::get_screen( int n ) const
    {
      dsI( 0 <= n && n < screen_count() );
      dsI( _p->_scrns );
      return _p->_scrns[n];
    }

    int display::default_screen_number() const
    {
      return XDefaultScreen( _p->_xdisplay );
    }

    int display::screen_count() const
    {
      return XScreenCount( _p->_xdisplay );
    }

    window::pointer display::default_root() const
    {
      window::pointer w( new window ); // TODO: avoid making a new instance of window
      //w->_p->_disp = display::pointer(const_cast<display*>(this));
      w->_p->_screen = this->default_screen();
      w->_p->_xwin = XDefaultRootWindow( _p->_xdisplay );
      return w;
    }
    
    void display::map( const window::pointer & win )
    {
      if ( /*!win->_p->_disp ||*/ !win->_p->_xwin ) {
        //win->_p->_disp = display::pointer(this);
        win->_p->_screen = this->default_screen();
        win->_p->create( win );
      }

      XMapWindow( _p->_xdisplay, win->_p->_xwin );
    }

    void display::unmap( const window::pointer & win )
    {
      XUnmapWindow( _p->_xdisplay, win->_p->_xwin );
    }

    bool display::has( const window::pointer & win )
    {
      if (!win->_p->_xwin) return false;
      return (_p->_winmap.find( win->_p->_xwin ) != _p->_winmap.end());
    }

    void display::pump_events()
    {
      _p->pump_events( event_pump::get_queue() );
    }
    
  }//namespace ui
}//namespace ds
