/**
 *    Copyright 2010-09-12 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/ui/display.hpp>
#include <ds/ui/screen.hpp>
#include <ds/ui/window.hpp>
#include <ds/debug.hpp>
#include "../window_impl.h"
#include "../screen_impl.h"
#include "../display_impl.h"
#include "paint_buffer.h"

namespace ds { namespace ui {

    bool display::IMPL::push_event( event_queue * eq, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
    {
      window_map_t::iterator wit = _winmap.find( hWnd );
      if (wit == _winmap.end())
        return false;

      window::pointer win( wit->second );
      dsI( win );

      //!< Unset the paint DC
      win->_p->_native_gc = NULL;

      switch (msg) {

      case WM_NCCREATE:
        break;

      case WM_CREATE:
        // TODO: ...
        break;

      case WM_CLOSE:
        dsL5("WM_CLOSE");
        // TODO: should notify the window of the close event
        break;

      case WM_DESTROY:
        dsL5("WM_DESTROY");
        // TODO: should notify the window of the destroy event
        win->_p->_native_win = NULL;
        win->_p->_screen.reset();
        _winmap.erase( wit->first );
        if ( _winmap.empty() ) {
          eq->push(new ds::event::quit);
        }
        break;

      case WM_PAINT: {
        // TODO: ...
        PAINTSTRUCT ps;
        HDC dc = ::BeginPaint(hWnd, &ps);
        win->_p->_native_gc = dc;

        /*
        event::window::expose *evt( new event::window::expose );
        evt->win = win.get();
        evt->param1 = ps.rcPaint.left;
        evt->param2 = ps.rcPaint.top;
        evt->param3 = ps.rcPaint.right - ps.rcPaint.left;
        evt->param4 = ps.rcPaint.bottom - ps.rcPaint.top;
        eq->push( evt );
        */
        event::window::expose evt;
        evt.win = win.get();
        evt.param1 = ps.rcPaint.left;
        evt.param2 = ps.rcPaint.top;
        evt.param3 = ps.rcPaint.right - ps.rcPaint.left;
        evt.param4 = ps.rcPaint.bottom - ps.rcPaint.top;

        // FIXME: using eq->push, and wait until it's been handled
        evt.win->on_expose( evt ); //!< NOTE: not pushed into the event_queue

        win->_p->_paint_buffer.flush( dc, NULL, NULL );//, &ps.rcPaint, &ps.rcPaint );
        win->_p->_native_gc = NULL;
        ::EndPaint(hWnd, &ps);
      } break;
        
      }//switch (msg)

      return false;
    }
    
  }//namespace ui
}//namespace ds
