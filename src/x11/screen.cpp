/**
 *    Copyright 2010-07-26 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/ui/screen.hpp>
#include <ds/ui/display.hpp>
#include <ds/ui/window.hpp>
#include <X11/Xlib.h>
#include "screen_impl.h"
#include "display_impl.h"
#include "window_impl.h"

#include <iostream>

namespace ds { namespace ui {

    screen::screen()
      : _p( new IMPL )
    {
    }

    screen::~screen()
    {
      std::clog<<"screen::~screen: "<<this<<std::endl;
      delete _p;
    }

    shared_object<display>::pointer_t screen::get_display() const
    {
      display::pointer_t disp( new display );
      disp->_p->xDisplay = XDisplayOfScreen( _p->xScreen );
      //disp->_p->screen = XScreenNumberOfScreen( _p->xScreen );
      return disp;
    }

    window::pointer_t screen::root() const
    {
      window::pointer_t w( new window );
      w->_p->disp = get_display();
      w->_p->xWindow = XRootWindowOfScreen( _p->xScreen );
      return w;
    }

    int screen::width() const
    {
      return XWidthOfScreen( _p->xScreen );
    }

    int screen::height() const
    {
      return XHeightOfScreen( _p->xScreen );
    }

    unsigned screen::black_pixel() const
    {
      return XBlackPixelOfScreen( _p->xScreen );
    }

    unsigned screen::white_pixel() const
    {
      return XWhitePixelOfScreen( _p->xScreen );
    }
    
  }//namespace ui
}//namespace ds