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
#include "window_impl.h"
#include "screen_impl.h"
#include "display_impl.h"

namespace ds { namespace ui {

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
      pointer d( new display );
      d->_p->open( d, (const char *) i._p );
      return d;
    }

    screen::pointer display::get_screen( int n ) const
    {
      return _p->get_screen( n );
    }

    int display::default_screen_number() const
    {
      return _p->default_screen_number();
    }

    screen::pointer display::default_screen() const
    {
      return get_screen( default_screen_number() );
    }

    int display::screen_count() const
    {
      return _p->screen_count();
    }

    window::pointer display::default_root() const
    {
      screen::pointer s = this->default_screen();

      dsI( s );

      window::pointer w = s->root();

      dsI( w );
      dsI( _p->is_default_root( w ) );

      return w;
    }
    
    void display::map( const window::pointer & win )
    {
      _p->map( win );
    }

    void display::unmap( const window::pointer & win )
    {
      _p->unmap( win );
    }

    bool display::has( const window::pointer & win )
    {
      return _p->has( win );
    }

    void display::pump_events()
    {
      _p->pump_events( event_pump::get_queue() );
    }

  }//namespace ui
}//namespace ds
