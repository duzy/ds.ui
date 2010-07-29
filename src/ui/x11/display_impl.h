/**
 *    Copyright 2010-07-25 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <map>

namespace ds { namespace ui {

    struct display::IMPL
    {
      typedef std::map<Window, window::pointer_t> window_map_t;

      Display * _xdisp;
      screen::pointer_t * _scrns;
      window_map_t _winmap;

      Atom WM_DELETE_WINDOW;

      IMPL()
        : _xdisp( NULL )
        , _scrns( NULL )
        , _winmap()
        , WM_DELETE_WINDOW( 0 )
      {
      }

      void init( display *disp, const char * name );
      void init_atoms();

      bool pending();
      void pump_events( event_queue * );
      void push_event( event_queue *, XEvent * event );
    };//struct display::IMPL

  }//namespace ui
}//namespace ds
