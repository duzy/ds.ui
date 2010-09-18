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
#include <boost/geometry/algorithms/make.hpp>
#include <ds/debug.hpp>

namespace ds { namespace ui {

    window::IMPL::IMPL( const screen::pointer & d )
      : _screen( d )
      , _image()
      , _dirty_rects()
      , _pended_updates()
      , _native_win( NULL )
      , _native_gc( NULL )
      , _paint_buffer()
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

      detail::window_creator wc;
      //wc.exStyle |= WS_EX_TRANSPARENT;
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

        // TODO: remove this window from the _dirty_wins list

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
      dsI( _native_win );

      RECT r;
      BOOL ok = ::GetWindowRect( _native_win, &r );
      dsI( ok );

      return boost::geometry::make<graphics::box>( r.left, r.top, r.right, r.bottom );
    }

    bool window::IMPL::create_image_if_needed( int w, int h )
    {
      if ( w <= _image.width() && h <= _image.height() ) {
        dsI( _paint_buffer.ptr() != NULL );
        return _paint_buffer.ptr() && _image.pixels() ;
      }

      _paint_buffer.destroy();
      _paint_buffer.create( w, h, 32 );
      dsI( _paint_buffer.ptr() );
      dsI( _paint_buffer.width() == w );
      dsI( _paint_buffer.height() == h );

      _image.create(w, h, ds::graphics::image::ARGB_8888_PIXEL, _paint_buffer.ptr());
      dsI( _image.is_valid() );
      dsI( _image.width() == w );
      dsI( _image.height() == h );
      dsI( _image.pixels() != NULL );
      dsI( _image.pixel_type() == ds::graphics::image::ARGB_8888_PIXEL );
      dsI( _image.pixel_size() == 4 );
      dsI( _paint_buffer.ptr() == _image.pixels() );
      
      return _paint_buffer.ptr() != NULL;
    }

    /**
     *  Put all dirty rects onto the screen.
     *
     *  @return true if any dirty rects has been push onto the screen.
     */
    bool window::IMPL::commit_updates()
    {
      if ( _pended_updates.empty() ) {
        dsL4("window is updated");
        return false;
      }

      HDC dc = _native_gc ? _native_gc : ::GetDC( _native_win ) ;

      dsL("commit-updates: "<<_pended_updates.size()-1);

      ds::graphics::box bound;
      box_list_t::const_iterator it = _pended_updates.begin();
      bound = *it++;                    dsI( it != _pended_updates.end() );

      // TODO: only flush pended update rects

      dsI( _paint_buffer.ptr() );
      
#if 0
      RECT r;
      r.left   = bound.left();
      r.top    = bound.top();
      r.right  = bound.right();
      r.bottom = bound.bottom();
      _paint_buffer.flush( dc, &r, &r );
#else
      bool ok = _paint_buffer.flush( dc, NULL, NULL );
      dsL4( "commited: "<<ok );
#endif

      _pended_updates.clear();
      return _pended_updates.empty();
    }
    
  }//namespace ui
}//namespace ds

