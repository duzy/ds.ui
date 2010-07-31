/**
 *    Copyright 2010-08-15 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/ui/window.hpp>
#include <ds/ui/screen.hpp>
#include <ds/ui/display.hpp>
#include <ds/graphics/canvas.hpp>
#include <ds/debug.hpp>
#include <boost/geometry/algorithms/make.hpp>
#include "x11/window_impl.h"

namespace ds { namespace ui {

    window::window()
      : _p( new IMPL(NULL) )
    {
    }

    window::window( const display::pointer & disp )
      : _p( new IMPL(disp->default_screen()) )
    {
      _p->create( this );
      disp->map( this );
    }

    window::~window()
    {
      _p->destroy();
      _p->_screen.reset();
      delete _p;
    }

    screen::pointer window::get_screen() const
    {
      return _p->_screen.lock();
    }

    void window::select_input(long mask)
    {
      _p->select_input( mask );
    }

    void window::destroy()
    {
      _p->destroy();
    }

    void window::show()
    {
      // TODO: ...
    }

    void window::hide()
    {
      // TODO: ...
    }

    void window::close()
    {
      /**
       *  1): send close event (to perform on_close)
       *  2): perform close
       */
      event::window::close evt;
      evt.win = this;
      this->on_close( evt );
    }

    void window::move( int x, int y )
    {
      // TODO: ...
    }

    void window::resize( int w, int h )
    {
      // TODO: ...
    }

    graphics::box window::rect() const
    {
      return _p->get_rect();
    }

    void window::on_exposed( const event::window::exposed & a )
    {
      ds::graphics::image * img( _p->get_image_for_render() );
      if ( img == NULL ) {
        return;
      }

      ds::graphics::box dr( boost::geometry::make<graphics::box>( a.x(), a.y(), a.width(), a.height() ) );
      if ( dr.is_empty() ) {
        dsE("empty dirty rect");
        return;
      }

      ds::graphics::canvas canvas( *img );
      canvas.clip( dr );

      this->on_render( canvas );

      _p->commit_image( dr );
    }

    void window::on_shown( const event::window::shown & a )
    {
    }

    void window::on_hidden( const event::window::hidden & a )
    {
    }

    /*
    void window::on_exposed( const event::window::exposed & a )
    {
      dsL("winact: "<<a.win);
    }
    */

    void window::on_moved( const event::window::moved & a )
    {
    }

    void window::on_resized( const event::window::resized & a )
    {
    }

    void window::on_minimized( const event::window::minimized & a )
    {
    }

    void window::on_maximized( const event::window::maximized & a )
    {
    }

    void window::on_restored( const event::window::restored & a )
    {
    }

    void window::on_enter( const event::window::enter & a )
    {
    }

    void window::on_leave( const event::window::leave & a )
    {
    }

    void window::on_focus( const event::window::focus & a )
    {
    }

    void window::on_close( const event::window::close & a )
    {
      do_close(); // Don't call 'close()'!
    }

    void window::do_close()
    {
      this->destroy(); // TODO: should be destroyed?
    }

    void window::on_render( ds::graphics::canvas & canvas )
    {
      dsL("TODO:render: "<<this);
      // TODO: draw each child view, support BSP view
    }
    
  }//namespace ui
}//namespace ds
