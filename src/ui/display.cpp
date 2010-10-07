/**
 *    Copyright 2010-08-07 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/
#include <ds/ui/window.hpp>
#include <ds/ui/screen.hpp>
#include <ds/ui/display.hpp>
#include "window_impl.h"
#include "screen_impl.h"
#include "display_impl.h"
#include <ds/ui/events.hpp>
#include <ds/event_queue.hpp>
#include <ds/debug.hpp>

namespace ds { namespace ui {

    display::display()
      : event_pump( NULL )
      , _p( new IMPL )
    {
    }

    display::~display()
    {
      dsI( _p );

      //!< destroy all mapped windows, all window::pointer references will
      //!< be invalid
      IMPL::window_map_t::iterator it = _p->_winmap.begin();
      for(; it != _p->_winmap.end(); ++it) {
        it->second->_p->destroy_natively( _p );
      }
      _p->_winmap.clear();

      delete _p;
    }

    display::pointer display::open( id i )
    {
      pointer d( new display );
      d->_p->open_natively( d, (const char *) i._p );
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
      window::IMPL * p = win->_p;
      if ( !p->_native_win ) {
        bool ok = p->create_natively( this, win );

        dsI( ok );
        dsI( p->_native_win != NULL );

        _p->_winmap.insert( std::make_pair( p->_native_win, win ) );

        dsI( this->has(win) );
      }

      // TODO: _winmap.insert( win ) if alread mapped(maybe by another display)?

      /**
       *  At this point the win may not really exists natively, the
       *  %map_win_natively is responsibly for creating the win
       */
      if ( _p->map_win_natively( win ) ) {
        // FIXME: a mapped window should be hidden by default
        //      dsI( !win->is_visible() );
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
      if ( !win->_p->_native_win ) return false;
      if ( _p->_winmap.find( win->_p->_native_win ) != _p->_winmap.end() )
        return _p->is_win_mapped_natively( win );
      return false;
    }

    void display::pump_events()
    {
      _p->pump_native_events( event_pump::get_queue() );

      // TODO: think about move this task somewhere better
      _p->redraw_dirty_wins( this );
    }

  }//namespace ui
}//namespace ds
