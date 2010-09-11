/**
 *    Copyright 2010-09-11 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/ui/screen.hpp>
#include <ds/ui/display.hpp>
#include <ds/ui/window.hpp>
#include "screen_impl.h"
#include "window_impl.h"
#include "display_impl.h"

namespace ds { namespace ui {

    screen::screen()
    {
    }

    screen::~screen()
    {
    }

    shared_object<display>::pointer screen::get_display() const
    {
      return NULL; //shared_object<display>::pointer
    }

    shared_object<window>::pointer screen::root() const
    {
      return NULL; //shared_object<window>::pointer
    }

    int screen::number() const
    {
      return 0;
    }

    int screen::width() const
    {
      return 0;
    }

    int screen::height() const
    {
      return 0;
    }

    unsigned screen::black_pixel() const
    {
      return 0x00000000;
    }

    unsigned screen::white_pixel() const
    {
      return 0xFFFFFFFF;
    }

    int screen::depth() const
    {
      return 24; //32;
    }

  }//namespace ui
}//namespace ds
