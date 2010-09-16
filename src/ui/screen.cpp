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
#include "window_impl.h"
#include "screen_impl.h"

namespace ds { namespace ui {

    screen::screen()
      : _p( new IMPL )
    {
    }

    screen::~screen()
    {
      delete _p;
    }

    shared_object<display>::pointer screen::get_display() const
    {
      return _p->get_display_ref().lock();
    }

    window::pointer screen::root() const
    {
      return _p->get_root_win( const_cast<screen*>(this) );
    }

    int screen::number() const
    {
      return _p->number();
    }

    int screen::width() const
    {
      return _p->width();
    }

    int screen::height() const
    {
      return _p->height();
    }

    unsigned screen::black_pixel() const
    {
      return _p->black_pixel();
    }

    unsigned screen::white_pixel() const
    {
      return _p->white_pixel();
    }

    int screen::depth() const
    {
      return _p->depth();
    }
    
  }//namespace ui
}//namespace ds
