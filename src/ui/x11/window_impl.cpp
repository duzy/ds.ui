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
      , _native_gc( NULL )
      , _ximage( NULL )
        //, _ximage_pixels( NULL )
    {
    }

    window::IMPL::~IMPL()
    {
    }

    Display * window::IMPL::x_display() const
    {
      screen::pointer scrn(_screen.lock());                    dsI(scrn);
      display::pointer disp(scrn->get_display());              dsI(disp);
      return disp->_p->_xdisplay;
    }

    bool window::IMPL::create_image_if_needed( int w, int h )
    {
      if ( w <= _image.width() && h <= _image.height() )
        return ( _ximage != NULL );

      _image.create(w, h, ds::graphics::image::PixelType(ds::graphics::canvas::sys_pixel_type()));
      dsI( _image.is_valid() );
      dsI( _image.width() == w );
      dsI( _image.height() == h );
      dsI( _image.pixels() != NULL );
      dsI( _image.pixel_type() == ds::graphics::canvas::sys_pixel_type() );
      dsI( _image.pixel_size() == 4 );
      
      if ( _ximage != NULL ) {
        _ximage->data = NULL;
        XDestroyImage( _ximage );
        _ximage = NULL;
      }

      screen::pointer scrn(_screen.lock());                     dsI(scrn);
      display::pointer disp(scrn->get_display());               dsI(disp);
      Display * xdisp = disp->_p->_xdisplay;                    dsI(xdisp);
      XVisualInfo *vi = scrn->_p->get_visual_info();            dsI( vi );

      dsI( xdisp != NULL );
      dsI( vi->visual != NULL );
      dsI( 0 < vi->depth );

      //char * pixels = reinterpret_cast<char*>(_ximage_pixels);
      char * pixels = reinterpret_cast<char*>( _image.pixels() );

      //_ximage_pixels = malloc( h * w * (vi->depth / 8) );
      _ximage = XCreateImage( xdisp, vi->visual, vi->depth, ZPixmap, 0, pixels,
                              _image.width(), _image.height(),
                              _image.pixel_size() * 8,/* bitmap pad */
                              _image.pixel_size() * w /* row bytes */ );

      if (_ximage==NULL) {
        //free(_ximage_pixels);
        //_ximage_pixels = NULL;
        dsE("failed create X image");
      } else {
        dsI( _image.width() == _ximage->width );
        dsI( _image.height() == _ximage->height );
      }

      dsL5("image: bitmap_unit = "<<_ximage->bitmap_unit);
      dsL5("image: bitmap_pad = "<<_ximage->bitmap_pad);
      dsL5("image: depth = "<<_ximage->depth);
      dsL5("image: bytes_per_line = "<<_ximage->bytes_per_line);
      dsL5("image: bits_per_pixel = "<<_ximage->bits_per_pixel);
      dsL5("image: red_mask = "<<_ximage->red_mask);
      dsL5("image: green_mask = "<<_ximage->green_mask);
      dsL5("image: blue_mask = "<<_ximage->blue_mask);

      return ( _ximage != NULL );
    }

    /*
    void window::IMPL::convert_pixels( int x, int y, int w, int h )
    {
      if (_image.width() <= x) return;
      if (_image.height() <= y) return;
      if (_image.width() < (x+w)) w = _image.width() - x;
      if (_image.height() < (y+h)) h = _image.height() - y;

      const uint8_t * s = _image.pixels();
      uint8_t * d = reinterpret_cast<uint8_t*>(_ximage_pixels);

      uint32_t rmask,  gmask,  bmask;
      rmask = _ximage->red_mask;
      gmask = _ximage->green_mask;
      bmask = _ximage->blue_mask;

      uint32_t rshift(0), gshift(0), bshift(0);
      while ((rmask & 1) == 0) { ++rshift; rmask >>= 1; }
      while ((gmask & 1) == 0) { ++gshift; gmask >>= 1; }
      while ((bmask & 1) == 0) { ++bshift; bmask >>= 1; }

      screen::pointer scrn(_screen.lock());                     dsI(scrn);
      XVisualInfo *vi = scrn->_p->get_visual_info();            dsI( vi );
      uint32_t ncolors = vi->colormap_size;
      uint32_t bpp = vi->depth / 8;

      // dsL("rmask: "<<rmask);
      // dsL("gmask: "<<gmask);
      // dsL("bmask: "<<bmask);
      // dsL("rshift: "<<rshift);
      // dsL("gshift: "<<gshift);
      // dsL("bshift: "<<bshift);
      dsL("color-bits: "<<_ximage->bits_per_pixel);
      dsL("colormap-size: "<<ncolors<<", "<<vi->visual->map_entries);
      dsEif(ncolors!=vi->visual->map_entries, "bad color map entries");

      s += y * _image.width() * _image.pixel_size();
      d += y * _image.width() * bpp;

      int red, green, blue, pixel, n, c;
      for (n = 0; n < h; ++n) {
        s += x * _image.pixel_size();
        d += x * bpp;
        for (c = 0; c < w; ++c) {
          red = *(s + 0), green = *(s + 1), blue = *(s + 2);

          red   = (red   & rmask) << rshift;
          green = (green & gmask) << gshift;
          blue  = (blue  & bmask) << bshift;

          pixel = red | green | blue;

          *reinterpret_cast<short*>(d) = static_cast<short>( pixel & 0xFFFF );

          s += _image.pixel_size();
          d += bpp;
        }
        s += (_image.width() - w) * _image.pixel_size();
        d += (_image.width() - w) * bpp;
      }
    }
    */

    bool window::IMPL::create_natively( const display::pointer & disp, const window::pointer & win )
    {
      dsI( !_native_win );

      screen::pointer scrn = disp->default_screen();            dsI( scrn );
      if ( !_screen.lock() /*!= scrn*/ )
        _screen = scrn; // save the reference of the screen

      int x(0), y(0), w(400), h(300), bw(0);

      XVisualInfo *vi = scrn->_p->get_visual_info();            dsI( vi );

      unsigned fc = scrn->black_pixel();
      unsigned bc = scrn->white_pixel();
 
      window::pointer root = scrn->root();                      dsI(root);

      Window pr = root->_p->_native_win;
      register Display * const xdisp( disp->_p->_xdisplay );

      //TODO: XCreateWindow(...);
      _native_win = XCreateSimpleWindow( xdisp, pr, x, y, w, h, bw, fc, bc );
      dsI( _native_win );

      XGCValues gcv;
      gcv.graphics_exposures = False;
      _native_gc = XCreateGC( xdisp, _native_win, GCGraphicsExposures, &gcv );
      dsI( _native_gc );

      /* Allow window to be deleted by the window manager */
      XSetWMProtocols( xdisp, _native_win, &disp->_p->WM_DELETE_WINDOW, 1 );

      int eventMask
        = KeyPressMask
        | KeyReleaseMask
        | ButtonPressMask
        | ButtonReleaseMask
        | EnterWindowMask
        | LeaveWindowMask
        | PointerMotionMask
        //| PointerMotionHintMask
        | Button1MotionMask
        | Button2MotionMask
        | Button3MotionMask
        | Button4MotionMask
        | ButtonMotionMask
        //| KeymapStateMask
        | ExposureMask
        | VisibilityChangeMask
        | StructureNotifyMask
        | ResizeRedirectMask
        | SubstructureNotifyMask
        | SubstructureRedirectMask
        | FocusChangeMask
        | PropertyChangeMask
        | ColormapChangeMask
        | OwnerGrabButtonMask
        ;
      XSelectInput( xdisp, _native_win, eventMask );

      return true;
    }

    void window::IMPL::destroy_natively( display::IMPL * disp )
    {
      dsI( disp );
      dsI( disp->_xdisplay );

      // if (_ximage_pixels) {
      //   free(_ximage_pixels);
      //   _ximage_pixels = NULL;
      // }

      if (_ximage) {
        _ximage->data = NULL;
        XDestroyImage(_ximage);
        _ximage = NULL;
      }

      if ( _native_win ) {
        dsL("destroy: "<<_native_win);
        XDestroyWindow( disp->_xdisplay, _native_win );
        _native_win = NULL;
      }
    }

    void window::IMPL::show_natively()
    {
      // TODO: ...
    }

    void window::IMPL::hide_natively()
    {
      // TODO: ...
    }

    void window::IMPL::select_input(long mask)
    {
      Display * xdisp  = x_display();
      dsI( xdisp );
      XSelectInput( xdisp, _native_win, mask );
    }

    ds::graphics::box window::IMPL::get_rect() const
    {
      Display * xdisp = x_display();                         dsI( xdisp );
      XWindowAttributes a;

      //std::memset( &a, 0, sizeof(a) );

      dsI( _native_win );
      Status ok = XGetWindowAttributes( xdisp, _native_win, &a );
      dsI( ok );
      dsI( !(a.width == 0 && a.height == 0) );

      return boost::geometry::make<graphics::box>( a.x, a.y, a.x+a.width, a.y+a.height );
    }

    ds::graphics::box window::IMPL::get_client_rect() const
    {
      Display * xdisp = x_display();                         dsI( xdisp );
      XWindowAttributes a;

      //std::memset( &a, 0, sizeof(a) );

      dsI( _native_win );
      Status ok = XGetWindowAttributes( xdisp, _native_win, &a );
      dsI( ok );
      dsI( !(a.width == 0 && a.height == 0) );

      return boost::geometry::make<graphics::box>( a.x, a.y, a.x+a.width, a.y+a.height );
    }

    /**
     *  push a box onto the screen
     */
    bool window::IMPL::commit_update_natively( const ds::graphics::box & b )
    {
      //convert_pixels( b.x(), b.y(), b.width(), b.height() );

      Display * xdisp = x_display();
      {
        // XCopyArea( _display->_xdisplay, _drawable, _native_win, _native_gc,
        // b.x, b.y, b.w, b.h,
        // b.x, b.y );
        XPutImage( xdisp, _native_win, _native_gc, _ximage,
                   b.x(), b.y(),
                   b.x(), b.y(), b.width(), b.height() );
      }

      return true;
    }

    bool window::IMPL::sync_updates_natively()
    {
      Display * xdisp = x_display();
      //XFlushGC( xdisp, _p->_native_gc );
      XFlush( xdisp );
      XSync( xdisp, False );
      return true;
    }    

  }// namespace ui
}//namespace ds
