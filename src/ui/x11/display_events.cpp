/**
 *    Copyright 2010-08-06 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
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
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "../window_impl.h"
#include "../screen_impl.h"
#include "../display_impl.h"
#include <boost/geometry/algorithms/make.hpp>

namespace ds { namespace ui {

    void display::IMPL::push_event( event_queue *eq, XEvent * event )
    {
      /* filter events catchs XIM events and sends them to the correct
         handler */
      if ( XFilterEvent(event, None) == True ) {
        return;
      }

      window_map_t::iterator wit = _winmap.find( event->xany.window );
      if (wit == _winmap.end()) {
        if (event->type == PropertyNotify) {
          //char *name = XGetAtomName(_xdisp, event->xproperty.atom);
          //dsD("property: "<<name);
          return;
        }
        dsE("no window: "<<event->xany.window<<" for "<<event->type);
        return;
      }

      window::pointer win( wit->second );
      dsI( win );

      // TODO: push parsed-event into the event_queue

      switch (event->type) {
      case PropertyNotify:
        // Should be used for debug reasons.
        break;

      case MapNotify: {
        event::window::shown *evt1( new event::window::shown );
        event::window::restored *evt2( new event::window::restored );
        evt1->win = win.get();
        evt2->win = win.get();
        eq->push( evt1 );
        eq->push( evt2 );
      } break;

      case UnmapNotify: {
        event::window::hidden *evt1( new event::window::hidden );
        event::window::minimized *evt2( new event::window::minimized );
        evt1->win = win.get();
        evt2->win = win.get();
        eq->push( evt1 );
        eq->push( evt2 );
      } break;

      case Expose: {
        dsL("expose");

        /**
         *  %_pended_updates holds a list of rects which has been drawn
         *  and pending for update onto the screen.
         */
        if ( win->_p->_pended_updates.empty() ) {
          /**
           *  If nothing drawn, we make a request for that.
           *
           *  FIXME: skip empty ps.rcPaint
           */
          graphics::box ub = boost::geometry::make<graphics::box>
            ( event->xexpose.x, event->xexpose.y, event->xexpose.x+event->xexpose.width, event->xexpose.y+event->xexpose.height );
          win->request_update( ub );
        }
        else {
          /**
           *  If any rects dirty(something has been drawn), we need to commit
           *  the image.
           */
          win->_p->commit_updates();
          dsI( win->_p->_pended_updates.empty() );
        }
        
        event::window::expose *evt( new event::window::expose );
        evt->win = win.get();
        evt->param1 = event->xexpose.x;
        evt->param2 = event->xexpose.y;
        evt->param3 = event->xexpose.width;
        evt->param4 = event->xexpose.height;
        eq->push( evt );
      } break;

      case ConfigureNotify: {
        event::window::moved *evt1( new event::window::moved );
        event::window::resized *evt2( new event::window::resized );
        evt1->win = win.get();
        evt1->param1 = event->xconfigure.x;
        evt1->param2 = event->xconfigure.y;
        evt2->win = win.get();
        evt2->param3 = event->xconfigure.width;
        evt2->param4 = event->xconfigure.height;
        eq->push( evt1 );
        eq->push( evt2 );
      } break;

      case KeyPress: {
        event::keyboard *evt( new event::keyboard );
        evt->window = win.get();
        evt->is_pressed = 1;
        evt->is_repeat = 0;
        evt->code = event->xkey.keycode; // TODO: layout[event->xkey.keycode]
        eq->push( evt );

        // TODO: convert key into text, send text_input
      } break;

      case KeyRelease: {
        event::keyboard *evt( new event::keyboard );
        evt->window = win.get();
        evt->is_pressed = 0;
        evt->is_repeat = 0;
        evt->code = event->xkey.keycode; // TODO: layout[event->xkey.keycode]
        eq->push( evt );
      } break;

      case KeymapNotify: { /* Generated upon EnterWindow and FocusIn */
        // TODO: ...
      } break;

      case MappingNotify: {
        // TODO: ...
      } break;

      case EnterNotify: {
        event::window::enter *evt( new event::window::enter );
        evt->win = win.get();
        evt->param1 = event->xcrossing.x;
        evt->param2 = event->xcrossing.y;
        eq->push( evt );
      } break;

      case LeaveNotify: {
        event::window::leave *evt( new event::window::leave );
        evt->win = win.get();
        evt->param1 = event->xcrossing.x;
        evt->param2 = event->xcrossing.y;
        eq->push( evt );
      } break;

      case FocusIn: {
        event::window::focus *evt( new event::window::focus );
        evt->win = win.get();
        evt->param1 = 1;
        eq->push( evt );
      } break;

      case FocusOut: {
        event::window::focus *evt( new event::window::focus );
        evt->win = win.get();
        evt->param1 = 0;
        eq->push( evt );
      } break;

      case MotionNotify: {
        // TODO: ...
      } break;

      case ButtonPress: {
        // TODO: ...
      } break;

      case ButtonRelease: {
        // TODO: ...
      } break;

      case SelectionRequest: {
        // TODO: ...
      } break;

      case SelectionNotify: {
        // TODO: ...
      } break;

      case ClientMessage:
        if (event->xclient.format == 32 &&
            event->xclient.data.l[0] == WM_DELETE_WINDOW) {
          /** Window closed */

          dsL4("WM_DELETE_WINDOW");

          event::window::close * evt(new event::window::close);
          evt->win = win.get();
          eq->push( evt );
        }
        break;

      case CreateNotify: {
        // TODO: ...
      } break;

      case DestroyNotify: {
        // TODO: should notify the window of the destroy event
        erase_destroyed_window( eq, event->xany.window, win );
      } break;

      default:
        //eq->push(new ds::event::test);
        break;
      }
    }

  }//namespace ui
}//namespace ds
