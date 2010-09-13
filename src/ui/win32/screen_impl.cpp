/**
 *    Copyright 2010-09-12 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/ui/screen.hpp>
#include <ds/ui/display.hpp>
#include <ds/ui/window.hpp>
#include "../screen_impl.h"

namespace ds { namespace ui {

    screen::IMPL::IMPL()
      : _display( NULL )
      , _root( NULL )
    {
    }

    screen::IMPL::~IMPL()
    {
    }

    window::pointer screen::IMPL::get_root_win( const screen::pointer & ) const
    {
      return NULL;
    }

    int screen::IMPL::number() const
    {
      return 0;
    }

    int screen::IMPL::width() const
    {
      return 0;
    }

    int screen::IMPL::height() const
    {
      return 0;
    }

    unsigned screen::IMPL::black_pixel() const
    {
      return 0x00000000;
    }

    unsigned screen::IMPL::white_pixel() const
    {
      return 0xFFFFFFFF;
    }

    int screen::IMPL::depth() const
    {
      return 24;
    }

  }//namespace ui
}//namespace ds
