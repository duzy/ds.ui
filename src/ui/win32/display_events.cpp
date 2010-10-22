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
#include <boost/geometry/algorithms/make.hpp>

namespace ds { namespace ui {

    bool display::IMPL::push_event( event_queue * eq, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
    {
      window_map_t::iterator wit = _winmap.find( hWnd );
      if (wit == _winmap.end())
        return false;

      window::pointer win( wit->second );                        dsI( win );

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
        erase_destroyed_window( eq, hWnd, win );
        break;

      case WM_ERASEBKGND:
        return 1; //!< ignore this

      case WM_PAINT: {
        PAINTSTRUCT ps;

        dsI( win->_p->_native_gc == NULL );
        win->_p->_native_gc = ::BeginPaint(hWnd, &ps);

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
            ( ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom );
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

        win->_p->_native_gc = NULL;
        ::EndPaint(hWnd, &ps);

        // TODO: send event::window::exposed
      } break;
        
      }//switch (msg)

      return false;
    }
    
  }//namespace ui
}//namespace ds
