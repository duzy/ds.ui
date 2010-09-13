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
      /**
       *  At this point the win may not really exists natively, the
       *  %map_win_natively is responsibly for creating the win
       */
      if ( _p->map_win_natively( win ) ) {
        // TODO: should insert into _winmap here?
        //       maybe should delay to the MappedNotify or WM_CREATE handler
        _p->_winmap.insert( std::make_pair( win->_p->_native_win, win ) );
      }
      else {
        dsE( "can't map window: "<<win->_p->_native_win );
      }
    }

    void display::unmap( const window::pointer & win )
    {
      _p->unmap_win_natively( win );
      // TODO: should erase from _winmap here?
      //       maybe should delay to the UnmappedNotify or WM_DESTROY handler
    }

    bool display::has( const window::pointer & win )
    {
      if ( _p->_winmap.find( win->_p->_native_win ) != _p->_winmap.end() )
        return _p->is_win_mapped_natively( win );
      return false;
    }

    void display::pump_events()
    {
      _p->pump_native_events( event_pump::get_queue() );
    }

  }//namespace ui
}//namespace ds
