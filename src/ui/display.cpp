/**
 *    Copyright 2010-08-07 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
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

namespace ds { namespace ui {

    screen::pointer_t display::default_screen() const
    {
      //int n( default_screen_number() );
      //dsI( 0 <= n && n < screen_count() );
      //dsI( _scrns );
      //return _scrns[n];
      return get_screen( default_screen_number() );
    }

  }//namespace ui
}//namespace ds
