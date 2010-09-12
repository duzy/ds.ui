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
#include "display_impl.h"
#include "screen_impl.h"
#include "window_impl.h"

namespace ds { namespace ui {

    void display::IMPL::open( const display::pointer & disp, const char * name )
    {
    }

    void display::IMPL::pump_events( event_queue * )
    {
    }

    void display::IMPL::map( const window::pointer & win )
    {
    }

    void display::IMPL::unmap( const window::pointer & win )
    {
    }

    bool display::IMPL::has( const window::pointer & win )
    {
      return true;
    }

    HWND display::IMPL::default_root() const
    {
      return NULL;
    }

    bool display::IMPL::is_default_root( const window::pointer & w ) const
    {
      return false;
    }
    
  }//namespace ui
}//namespace ds
