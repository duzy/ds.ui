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

namespace ds { namespace ui {

    bool display::IMPL::push_event( event_queue * eq, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
    {
      window_map_t::iterator wit = _winmap.find( hWnd );
      if (wit == _winmap.end())
        return false;

      window::pointer win( wit->second );
      dsI( win );

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
        
      }//switch (msg)

      // TODO: push events into eq
      return false;
    }
    
  }//namespace ui
}//namespace ds
