/**
 *    Copyright 2010-09-11 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <map>
#include <windows.h>

namespace ds { namespace ui {

    struct display::IMPL
    {
      typedef std::map<HWND, window::pointer> window_map_t;

      screen::pointer * _scrns;
      window_map_t _winmap;

      IMPL()
        : _scrns( NULL )
        , _winmap()
      {
      }

      ~IMPL()
      {
        delete [] _scrns;
      }

      void open( const display::pointer & disp, const char * name );

      void pump_events( event_queue * );

      void map( const window::pointer & win );
      void unmap( const window::pointer & win );

      bool has( const window::pointer & win );

      int screen_count() const
      {
        return 1;
      }

      int default_screen_number() const
      {
        return 0;
      }

      screen::pointer get_screen( int n ) const
      {
        if ( 0 <= n && n < screen_count() ) {
          dsI( _scrns );
          return _scrns[n];
        }
        return NULL;
      }

      HWND default_root() const;
      bool is_default_root( const window::pointer & w ) const;
    };//struct display::IMPL
    
  }//namespace ui
}//namespace ds
