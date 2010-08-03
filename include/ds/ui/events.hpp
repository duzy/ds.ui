/**
 *    Copyright 2010-07-31 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#ifndef __DS_UI_EVENTS_HPP____by_Duzy_Chan__
#define __DS_UI_EVENTS_HPP____by_Duzy_Chan__ 1
#       include <ds/event.hpp>
#       include <ds/event_loop.hpp>
#       include <ds/shared_object.hpp>

namespace ds {
  
  class event_queue;

  namespace ui {
    namespace event
    {
      enum { TypeValueStart = 100 };

      struct quit : ds::event::quit {};

      struct window : ds::event::sub<window, TypeValueStart + 1>
      {
      };

      struct keyboard : ds::event::sub<keyboard, TypeValueStart + 2>
      {
      };

      struct mouse_motion : ds::event::sub<mouse_motion, TypeValueStart + 3>
      {
      };

      struct mouse_button : ds::event::sub<mouse_button, TypeValueStart + 4>
      {
      };

      struct mouse_wheel : ds::event::sub<mouse_wheel, TypeValueStart + 5>
      {
      };
    }//namespace event

    enum NamedEventType //: int
      {
        EVENT_QUIT      = event::quit::TypeValue,
        EVENT_WINDOW    = event::window::TypeValue,
        EVENT_KEYBOARD  = event::keyboard::TypeValue,
      };

    class display;

    /**
     *  @brief UI event loop
     */
    class event_loop : public ds::event_loop
    {
    public:
      event_loop(const shared_object<display>::pointer_t &);
      
      virtual ~event_loop();

    protected:
      virtual void should_pump_events();
      virtual void on_event(const ds::event &);

    private:
      shared_object<display>::pointer_t _display;
    };//class event_loop

    event_queue * get_event_queue();
  }//namespace ui
}//namespace ds

#endif//__DS_UI_EVENTS_HPP____by_Duzy_Chan__
