/**
 *    Copyright 2010-08 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/ui/events.hpp>
#include <ds/ui/display.hpp>
#include <ds/event_queue.hpp>
#include <ds/debug.hpp>

namespace ds { namespace ui {

    bool handle_window_event( const display::pointer &, const ds::event & );
    bool handle_keyboard_event( const display::pointer &, const ds::event & );

    event_loop::event_loop(const display::pointer & disp)
      : boost::base_from_member<event_queue>()
      , ds::event_loop( &(boost::base_from_member<event_queue>::member) )
      , _display( disp )
    {
      //dsD("loop: "<<this);
      disp->start( &(boost::base_from_member<event_queue>::member) );
    }

    event_loop::~event_loop()
    {
      //dsD("loop: "<<this);
    }

    void event_loop::loop_in()
    {
      /**
       *  Pump events out of the display will actually push the events into
       *  the event queue, since at this point it's in the same thread as the
       *  event loop(see ds::event_loop::run), it must be safe to 'wait'
       *  messages.
       */
      _display->pump(); // Pump some events
    }

    void event_loop::on_event(const ds::event & evt)
    {
      // TODO: dispatch UI events
      // TODO: close _display on quit event
      //dsD("event: "<<evt.type);
      switch (evt.type) {
      case event::window::TypeValue:
        handle_window_event(_display, evt);
        break;
      case event::keyboard::TypeValue:
        handle_keyboard_event(_display, evt);
        break;
      case event::text_input::TypeValue:
        // TODO: ...
        break;
      case event::text_composing::TypeValue:
        // TODO: ...
        break;
      case event::mouse_motion::TypeValue:
        // TODO: ...
        break;
      case event::mouse_button::TypeValue:
        // TODO: ...
        break;
      case event::mouse_wheel::TypeValue:
        // TODO: ...
        break;
      default:
        ds::event_loop::on_event(evt);
        break;
      }
    }
    
  }//namespace ui
}//namespace ds
