/**
 *    Copyright 2010-08-15 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/ui/window.hpp>
#include <ds/graphics/canvas.hpp>
#include <ds/debug.hpp>

namespace ds { namespace ui {

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

    void window::on_shown( const event::window::shown & a )
    {
      dsL("winact: "<<a.win);
    }

    void window::on_hidden( const event::window::hidden & a )
    {
      dsL("winact: "<<a.win);
    }

    /*
    void window::on_exposed( const event::window::exposed & a )
    {
      dsL("winact: "<<a.win);
    }
    */

    void window::on_moved( const event::window::moved & a )
    {
      dsL("winact: "<<a.win);
    }

    void window::on_resized( const event::window::resized & a )
    {
      dsL("winact: "<<a.win);
    }

    void window::on_minimized( const event::window::minimized & a )
    {
      dsL("winact: "<<a.win);
    }

    void window::on_maximized( const event::window::maximized & a )
    {
      dsL("winact: "<<a.win);
    }

    void window::on_restored( const event::window::restored & a )
    {
      dsL("winact: "<<a.win);
    }

    void window::on_enter( const event::window::enter & a )
    {
      dsL("winact: "<<a.win);
    }

    void window::on_leave( const event::window::leave & a )
    {
      dsL("winact: "<<a.win);
    }

    void window::on_focus( const event::window::focus & a )
    {
      dsL("winact: "<<a.win);
    }

    void window::on_close( const event::window::close & a )
    {
      dsL("winact: "<<a.win);
      do_close(); // Don't call 'close()'!
    }

    void window::do_close()
    {
      this->destroy(); // TODO: should be destroyed?
    }

    void window::on_render( ds::graphics::canvas & canvas )
    {
      dsL("render: "<<this);
      // TODO: draw each child view, support BSP view
    }
    
  }//namespace ui
}//namespace ds
