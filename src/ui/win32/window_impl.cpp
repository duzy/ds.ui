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
#include "window_creator.h"
#include <ds/debug.hpp>

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

    bool window::IMPL::create( const display::pointer & disp, const window::pointer & win )
    {
      dsI( !_native_win );
      dsI( !_screen.lock() );

      detail::window_creator wc;
      HWND hwnd = wc.create( disp->_p );

      dsI( hwnd );

      _screen = disp->default_screen();
      _native_win = hwnd;
     
      return _native_win != NULL;
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

