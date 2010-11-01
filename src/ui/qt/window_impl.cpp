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
#include <QtGui/QPainter>

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

      _image.create(w, h, ds::graphics::image::PixelType(ds::graphics::canvas::sys_pixel_type()));
      dsI( _image.is_valid() );
      dsI( _image.width() == w );
      dsI( _image.height() == h );
      dsI( _image.pixels() != NULL );
      dsI( _image.pixel_type() == ds::graphics::canvas::sys_pixel_type() );
      dsI( _image.pixel_size() == 4 );
      
      return false;
    }

    bool window::IMPL::create_natively( const display::pointer & disp, const window::pointer & win )
    {
      dsI( !_native_win );

      screen::pointer scrn = disp->default_screen();            dsI( scrn );
      if ( !_screen.lock() /*!= scrn*/ )
        _screen = scrn; // save the reference of the screen

      _native_win = new QWidget( NULL, Qt::Window );
      
      return _native_win != NULL;
    }

    void window::IMPL::destroy_natively( display::IMPL * disp )
    {
      dsI( disp );
      delete _native_win;
      _native_win = NULL;
    }

    void window::IMPL::show_natively()
    {
      _native_win->show();
    }

    void window::IMPL::hide_natively()
    {
      _native_win->hide();
    }

    void window::IMPL::select_input(long mask)
    {
    }

    ds::graphics::box window::IMPL::get_rect() const
    {
      QRect r = _native_win->frameGeometry(); //rect();
      return boost::geometry::make<graphics::box>( r.x(), r.y(), r.x()+r.width(), r.y()+r.height() );
    }

    ds::graphics::box window::IMPL::get_client_rect() const
    {
      QRect r = _native_win->rect();
      return boost::geometry::make<graphics::box>( r.x(), r.y(), r.x()+r.width(), r.y()+r.height() );
    }

    bool window::IMPL::commit_update_natively( const ds::graphics::box & b )
    {
      QImage::Format fmt = QImage::Format_Invalid;

#     if 0
      switch (_image.pixel_type()) {
      }
#     else
      fmt = QImage::Format_ARGB32;
#     endif

      // TODO: perform color format conversion here

      QImage m( _image.pixels(), _image.width(), _image.height(), fmt );

      QPainter painter( _native_win );
      painter.drawImage( b.x(), b.y(), m, b.x(), b.y(), b.width(), b.height() );
      return true;
    }

    bool window::IMPL::sync_updates_natively()
    {
      return true;
    }

  }// namespace ui
}//namespace ds
