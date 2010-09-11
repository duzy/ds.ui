/**
 *    Copyright 2010-09-11 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/ui/display.hpp>
#include <ds/ui/screen.hpp>
#include <ds/ui/window.hpp>
#include "display_impl.h"
#include "screen_impl.h"
#include "window_impl.h"

namespace ds { namespace ui {

    display::id::id( void *p ) : _p( p ) {}

    display::display()
      : event_pump( NULL )
      , _p( new IMPL )
    {
    }

    display::~display()
    {
      delete _p;
    }

    display::pointer display::open( id i )
    {
      return NULL;
    }

    shared_object<screen>::pointer display::get_screen( int index ) const
    {
      if (index == 0) {
        // TODO: ...
      }
      return NULL; //shared_object<screen>::pointer(NULL);
    }

    int display::default_screen_number() const
    {
      return 0;
    }

    int display::screen_count() const
    {
      return 1;
    }

    shared_object<window>::pointer display::default_root() const
    {
      // TODO: ...
    }

    void display::map( const shared_object<window>::pointer& win )
    {
      // TODO: ...
    }

    void display::unmap( const shared_object<window>::pointer& win )
    {
      // TODO: ...
    }

    bool display::has( const shared_object<window>::pointer& win )
    {
      // TODO: ...
      return true;
    }

    void display::pump_events()
    {
      // TODO: ...
    }
    
  }//namespace ui
}//namespace ds

