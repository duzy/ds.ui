/**
 *    Copyright 2010-07-25 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/ui/window.hpp>
#include <ds/ui/display.hpp>
#include <ds/ui/screen.hpp>
#include <ds/graphics/image.hpp>
#include <ds/graphics/canvas.hpp>
#include <ds/graphics/box.hpp>
#include <ds/debug.hpp>
#include <boost/geometry/algorithms/make.hpp>
#include "../window_impl.h"
#include "../screen_impl.h"
#include "../display_impl.h"

namespace ds { namespace ui {

    window::IMPL::IMPL( const screen::pointer & d )
      : _screen( d )
      , _image()
      , _dirty_region()
      , _native_win( NULL )
    {
    }

    window::IMPL::~IMPL()
    {
      delete _native_win;
    }

    bool window::IMPL::create_image_if_needed( int w, int h )
    {
      if ( w <= _image.width() && h <= _image.height() )
        return true; //( _ximage != NULL );

      _image.create(w, h, ds::graphics::image::ARGB_8888_PIXEL);
      dsI( _image.is_valid() );
      dsI( _image.width() == w );
      dsI( _image.height() == h );
      dsI( _image.pixels() != NULL );
      dsI( _image.pixel_type() == ds::graphics::image::ARGB_8888_PIXEL );
      dsI( _image.pixel_size() == 4 );
      
      return false;
    }

    bool window::IMPL::create( const display::pointer & disp, const window::pointer & win )
    {
      dsI( !_native_win );
      //dsI( !_screen );

      screen::pointer scrn = disp->default_screen();            dsI( scrn );
      if ( !_screen.lock() /*!= scrn*/ )
        _screen = scrn; // save the reference of the screen

      _native_win = new QWidget( NULL, Qt::Window );
      //_native_win->show();
      
      return _native_win != NULL;
    }

    void window::IMPL::destroy( display::IMPL * disp )
    {
      dsI( disp );
    }

    void window::IMPL::select_input(long mask)
    {
    }

    ds::graphics::box window::IMPL::get_rect() const
    {
      return boost::geometry::make<graphics::box>( 0, 0, 0, 0 );
    }

    bool window::IMPL::commit_updates()
    {
      if ( _pended_updates.empty() ) {
        dsL4("window is updated");
        return false;
      }

      dsL("commit-updates: "<<_pended_updates.size()-1);

      ds::graphics::box bound = _pended_updates.bounds();

      if ( bound.empty() ) {
        dsE("empty update bounds");
        return false;
      }

      ds::graphics::region::const_iterator it = _pended_updates.begin();
      ds::graphics::region::const_iterator const end = _pended_updates.end();
      for (; it != end; ++it) {
        // ...
      }
      
      _pended_updates.clear();

      //XFlushGC( xdisp, _p->_native_gc );
      //XFlush( xdisp );
      //XSync( xdisp, False );
      return true;
    }

  }// namespace ui
}//namespace ds
