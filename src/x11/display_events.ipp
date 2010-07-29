/**
 *    Copyright 2010-07-25 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

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
        dsD("no window: "<<event->xany.window<<" for "<<event->type);
        return;
      }

      window::pointer_t win( wit->second );
      dsI( win );

      // TODO: push parsed-event into the event_queue

      switch (event->type) {
      case PropertyNotify:
        // Should be used for debug reasons.
        break;

      case UnmapNotify: {
        // TODO: window hidden
        // TODO: window minimized
      } break;

      case MapNotify: {
        // TODO: window shown
        // TODO: window restored
      } break;

      case Expose: {
        // TODO: ...
      } break;

      case ConfigureNotify: {
        // TODO: window moved: event->xconfigure.x, event->xconfigure.y
        // TODO: window resized: event->xconfigure.width, event->xconfigure.height
      } break;

      case KeyPress: {
        // TODO: ...
      } break;

      case KeyRelease: {
        // TODO: ...
      } break;

      case KeymapNotify: { /* Generated upon EnterWindow and FocusIn */
        // TODO: ...
      } break;

      case MappingNotify: {
        // TODO: ...
      } break;

      case EnterNotify: {
        // TODO: ...
      } break;

      case LeaveNotify: {
        // TODO: ...
      } break;

      case FocusIn: {
        // TODO: ...
      } break;

      case FocusOut: {
        // TODO: ...
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

          dsD("WM_DELETE_WINDOW");
          win->close();
        }
        break;

      case CreateNotify: {
        // TODO: ...
      } break;

      case DestroyNotify: {
        win->_p->_xwin = NULL;
        //win->_p->_disp = NULL;
        _winmap.erase( wit->first );
        if ( _winmap.empty() ) {
          eq->push(new ds::event::quit);
        }
      } break;

      default:
        eq->push(new ds::event::test);
        break;
      }
    }

  }//namespace ui
}//namespace ds
