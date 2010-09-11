/**
 *    Copyright 2010-09-11 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/ui/window.hpp>
#include <ds/ui/screen.hpp>
#include <ds/ui/display.hpp>
#include <ds/graphics/box.hpp>
#include "window_impl.h"
#include "display_impl.h"
#include "screen_impl.h"

namespace ds { namespace ui {

    window::window()
    {
    }

    window::window( const shared_object<display>::pointer & disp )
    {
    }

    window::~window()
    {
    }

    shared_object<screen>::pointer window::get_screen() const
    {
      return NULL;
    }

    void window::select_input(long mask)
    {
    }

    void window::destroy()
    {
    }

    void window::show()
    {
    }

    void window::hide()
    {
    }

    void window::move( int x, int y )
    {
    }

    void window::resize( int w, int h )
    {
    }

    graphics::box window::rect() const
    {
      graphics::box r;
      return r;
    }

  }//namespace ui
}//namespace ds
