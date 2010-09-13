/**
 *    Copyright 2010-09-12 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/ui/window.hpp>
#include <ds/ui/screen.hpp>
#include <ds/ui/display.hpp>
#include "../window_impl.h"
#include "../screen_impl.h"

namespace ds { namespace ui {

    window::IMPL::IMPL( const screen::pointer & d )
      : _screen( d )
      , _image()
      , _dirty_rects()
      , _native_win( NULL )
      , _native_gc( NULL )
    {
    }

    window::IMPL::~IMPL()
    {
    }

    void window::IMPL::create( const window::pointer & )
    {
    }

    void window::IMPL::destroy()
    {
    }

    void window::IMPL::convert_pixels( int x, int y, int w, int h )
    {
    }

    void window::IMPL::select_input(long mask)
    {
    }

    ds::graphics::box window::IMPL::get_rect() const
    {
    }

    ds::graphics::image * window::IMPL::get_image_for_render()
    {
    }

    bool window::IMPL::commit_image( const ds::graphics::box & dr )
    {
      return false;
    }
    
  }//namespace ui
}//namespace ds

