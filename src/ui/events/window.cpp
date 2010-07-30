/**
 *    Copyright 2010-08-15 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/ui/events.hpp>
#include <ds/ui/display.hpp>
#include <ds/ui/window.hpp>
#include <ds/debug.hpp>

namespace ds { namespace ui {

    struct window::on
    {
#   define ON_ACT(ACT)                                          \
      template<class A>                                         \
      static inline void ACT( const event::window & e )         \
      {                                                         \
        static const A & a = reinterpret_cast<const A &>(e);    \
        e.win->on_##ACT(a);                                     \
      }                                                         \
      template<class A>                                         \
      static inline void ACT( const A & a )                     \
      {                                                         \
        a.win->on_##ACT(a);                                     \
      }

      ON_ACT(shown);
      ON_ACT(hidden);
      ON_ACT(exposed);
      ON_ACT(moved);
      ON_ACT(resized);
      ON_ACT(minimized);
      ON_ACT(maximized);
      ON_ACT(restored);
      ON_ACT(enter);
      ON_ACT(leave);
      ON_ACT(focus);
      ON_ACT(close);

#   undef ON_ACT
    };//struct window::on

    bool handle_window_event( const display::pointer & disp, const ds::event & evt )
    {
      const event::window & e = reinterpret_cast<const event::window&>(evt);
      if (e.win == NULL) {
        dsE("No bound window: action="<<e.action);
        return false;
      }

      switch (e.action) {
      case event::window::shown::ActionValue: {
        const event::window::shown & a = reinterpret_cast<const event::window::shown&>(e);
        window::on::shown(a);
      } break;

      case event::window::hidden::ActionValue: {
        const event::window::hidden & a = reinterpret_cast<const event::window::hidden&>(e);
        window::on::hidden(a);
      } break;

      case event::window::exposed::ActionValue: {
        const event::window::exposed & a = reinterpret_cast<const event::window::exposed&>(e);
        window::on::exposed(a);
      } break;

      case event::window::moved::ActionValue: {
        const event::window::moved & a = reinterpret_cast<const event::window::moved&>(e);
        window::on::moved(a);
      } break;

      case event::window::resized::ActionValue: {
        const event::window::resized & a = reinterpret_cast<const event::window::resized&>(e);
        window::on::resized(a);
      } break;

      case event::window::minimized::ActionValue: {
        const event::window::minimized & a = reinterpret_cast<const event::window::minimized&>(e);
        window::on::minimized(a);
      } break;

      case event::window::maximized::ActionValue: {
        const event::window::maximized & a = reinterpret_cast<const event::window::maximized&>(e);
        window::on::maximized(a);
      } break;

      case event::window::restored::ActionValue: {
        const event::window::restored & a = reinterpret_cast<const event::window::restored&>(e);
        window::on::restored(a);
      } break;

      case event::window::enter::ActionValue: {
        const event::window::enter & a = reinterpret_cast<const event::window::enter&>(e);
        window::on::enter(a);
      } break;

      case event::window::leave::ActionValue: {
        const event::window::leave & a = reinterpret_cast<const event::window::leave&>(e);
        window::on::leave(a);
      } break;

      case event::window::focus::ActionValue: {
        const event::window::focus & a = reinterpret_cast<const event::window::focus&>(e);
        window::on::focus(a);
      } break;

      case event::window::close::ActionValue: {
        const event::window::close & a = reinterpret_cast<const event::window::close&>(e);
        window::on::close(a);
      } break;
      }//switch
      
      return false;
    }
    
  }//namespace ui
}//namespace ds
