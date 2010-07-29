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
#       include <ds/event_queue.hpp> // TODO: avoid include this
#       include <ds/shared_object.hpp>
#       include <boost/utility/base_from_member.hpp>
#       include <string>

namespace ds { namespace ui {
    class window;
    class display;

    namespace event
    {
      enum { TypeValueStart = 100 };

      struct quit : ds::event::quit {};

      //======================================================================

      struct window : ds::event::sub<window, TypeValueStart + 1>
      {
        ui::window * win;
        int action;
        
        int param1; // should not use it directly
        int param2; // should not use it directly
        int param3; // should not use it directly
        int param4; // should not use it directly

        explicit window(int a) : win( NULL ), action(a) {}

        template<class Action, int> struct act;

        struct shown;
        struct hidden;
        struct exposed;
        struct moved;
        struct resized;
        struct minimized;
        struct maximized;
        struct restored;
        struct enter;
        struct leave;
        struct focus;
        struct close;
      };

      template<class Action, int ActionValue>
      struct window::act : window
      {
        act() : window(ActionValue) {}
      };
      
      struct window::shown : window::act<window::shown, 1>
      {
      };

      struct window::hidden : window::act<window::hidden, 2>
      {
      };

      struct window::exposed : window::act<window::exposed, 3>
      {
        int x() const { return param1; }
        int y() const { return param2; }
        int width() const { return param3; }
        int height() const { return param4; }
      };

      struct window::moved : window::act<window::moved, 4>
      {
        int x() const { return param1; }
        int y() const { return param2; }
      };

      struct window::resized : window::act<window::resized, 5>
      {
        int width() const { return param3; }
        int height() const { return param4; }
      };

      struct window::minimized : window::act<window::minimized, 6>
      {
      };

      struct window::maximized : window::act<window::maximized, 7>
      {
      };

      struct window::restored : window::act<window::restored, 8>
      {
      };

      struct window::enter : window::act<window::enter, 9>
      {
        int x() const { return param1; }
        int y() const { return param2; }
      };

      struct window::leave : window::act<window::leave, 10>
      {
        int x() const { return param1; }
        int y() const { return param2; }
      };

      struct window::focus : window::act<window::focus, 11>
      {
        bool has_focus() const { return param1 != 0; }
      };

      struct window::close : window::act<window::close, 12>
      {
      };

      //======================================================================

      struct keyboard : ds::event::sub<keyboard, TypeValueStart + 2>
      {
        uint8_t is_pressed : 1;
        uint8_t is_repeat : 1;
        uint32_t code;
      };

      struct text_input  : ds::event::sub<text_input, TypeValueStart + 3>
      {
        ui::window * window;
        std::string text; // UTF8
      };

      struct text_composing  : ds::event::sub<text_composing, TypeValueStart + 4>
      {
        ui::window * window;
        std::string text; // UTF8
        int start;
        int length;
      };

      struct mouse_motion : ds::event::sub<mouse_motion, TypeValueStart + 5>
      {
        ui::window * window;
        uint8_t state;
        int x;
        int y;
        int rel_x;
        int rel_y;
      };

      struct mouse_button : ds::event::sub<mouse_button, TypeValueStart + 6>
      {
        ui::window * window;
        uint8_t state;
        uint8_t button;
        int x;
        int y;
      };

      struct mouse_wheel : ds::event::sub<mouse_wheel, TypeValueStart + 7>
      {
        ui::window * window;
        int x; /** The amount scrolled horizontally */
        int y; /** The amount scrolled vertically */
      };
    }//namespace event

    /**
     *  @brief UI event loop
     */
    class event_loop
      : public boost::base_from_member<event_queue>
      , public ds::event_loop
    {
    public:
      event_loop(const shared_object<display>::pointer &);
      
      virtual ~event_loop();

    protected:
      virtual void loop_in();
      virtual void on_event(const ds::event &);

    private:
      shared_object<display>::pointer _display;
    };//class event_loop

  }//namespace ui
}//namespace ds

#endif//__DS_UI_EVENTS_HPP____by_Duzy_Chan__
