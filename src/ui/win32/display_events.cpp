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

      // TODO: push events into eq
      return false;
    }
    
  }//namespace ui
}//namespace ds
