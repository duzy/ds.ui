/**
 *    Copyright 2010-07-25 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <map>
#ifdef _WIN32
#   include <windows.h>
#elif defined(X11)
#   include <X11/Xlib.h>
#else
#   error unsupported platform
#endif

namespace ds { namespace ui {

    struct display::IMPL
    {
      typedef std::map<native_window_t, window::pointer> window_map_t;

      IMPL();
      ~IMPL();

      void open( const display::pointer & disp, const char * name );
      void pump_events( event_queue * );
      void map( const window::pointer & win );
      void unmap( const window::pointer & win );
      bool has( const window::pointer & win );
      int screen_count() const;
      int default_screen_number() const;
      screen::pointer get_screen( int n ) const;
      native_window_t default_root() const;
      bool is_default_root( const window::pointer & w ) const;

      screen::pointer * _scrns;
      window_map_t _winmap;

#ifdef _WIN32
#elif defined(X11)
      Display * _xdisplay;
      Atom WM_DELETE_WINDOW;
      void init_atoms();
      bool pending();
      void push_event( event_queue *, XEvent * event );
#else
#   error unsupported platform
#endif
    };//struct display::IMPL

  }//namespace ui
}//namespace ds
