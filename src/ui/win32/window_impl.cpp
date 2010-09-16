/**
 *    Copyright 2010-09-12 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#define WIN32_LEAN_AND_MEAN
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
      //dsI( !_screen.lock() );

      screen::pointer scrn = disp->default_screen();            dsI( scrn );
      if ( !_screen.lock() /*!= scrn*/ ) _screen = scrn;

      static HWND root = NULL;
      /*
      if ( root == NULL ) {
        detail::window_creator wc;
        wc.exStyle = wc.style = 0;
        wc.windowName = L"ds::ui::window::root";
        root = wc.create( disp->_p );
        dsI( root );
      }
      */

      detail::window_creator wc;
      wc.style |= WS_VISIBLE;
      wc.param = reinterpret_cast<LPVOID>( disp.get() );
      HWND hwnd = wc.create( disp->_p );

      dsI( hwnd != NULL );

      //::SetWindowLong( hwnd, GWL_USERDATA, reinterpret_cast<LONG>(disp.get()) );

      _native_win = hwnd;

      return _native_win != NULL;
    }

    void window::IMPL::destroy( display::IMPL * disp )
    {
      dsI( disp );

      /**
       *  the display will invoke this when it's closed/destroyed
       */
      if ( _native_win ) {
        dsL("destroy: "<<_native_win);

        // FIXME: let WM_DESTROY handler do this?
        ::SetWindowLong( _native_win, GWL_USERDATA, NULL );

        ::DestroyWindow( _native_win );
        _native_win = NULL;
      }
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

