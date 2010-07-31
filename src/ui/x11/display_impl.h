/**
 *    Copyright 2010-07-25 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <map>
#include <X11/Xlib.h>

namespace ds { namespace ui {

    struct display::IMPL
    {
      typedef std::map<Window, window::pointer> window_map_t;

      Display * _xdisplay;
      screen::pointer * _scrns;
      window_map_t _winmap;

      Atom WM_DELETE_WINDOW;

      IMPL()
        : _xdisplay( NULL )
        , _scrns( NULL )
        , _winmap()
        , WM_DELETE_WINDOW( 0 )
      {
      }

      ~IMPL()
      {
        if ( _xdisplay ) {
          XCloseDisplay( _xdisplay );
          _winmap.clear();
        }

        delete [] _scrns;
      }

      void init_atoms();

      void open( const display::pointer & disp, const char * name );

      bool pending();
      void pump_events( event_queue * );
      void push_event( event_queue *, XEvent * event );

      void map( const window::pointer & win );
      void unmap( const window::pointer & win );

      bool has( const window::pointer & win );

      int screen_count() const
      {
        dsI( _xdisplay );
        return XScreenCount( _xdisplay );
      }

      int default_screen_number() const
      {
        dsI( _xdisplay );
        return XDefaultScreen( _xdisplay );
      }

      screen::pointer get_screen( int n ) const
      {
        if ( 0 <= n && n < screen_count() ) {
          dsI( _scrns );
          return _scrns[n];
        }
        return NULL;
      }

      Window default_root() const;
      bool is_default_root( const window::pointer & w ) const;
    };//struct display::IMPL

  }//namespace ui
}//namespace ds
