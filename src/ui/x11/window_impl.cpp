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
#include "../display_impl.h"

namespace ds { namespace ui {

    window::IMPL::IMPL( const screen::pointer & d )
      : _screen( d )
      , _image()
      , _dirty_rects()
      , _native_win( NULL )
      , _native_gc( NULL )
      , _vi()
      , _ximage( NULL )
      , _ximage_pixels( NULL )
    {
      std::memset( &_vi, 0, sizeof(_vi) );
    }

    window::IMPL::~IMPL()
    {
      if (_ximage_pixels) {
        free(_ximage_pixels);
      }
      if (_ximage) {
        _ximage->data = NULL;
        XDestroyImage(_ximage);
      }
    }

    Display * window::IMPL::x_display() const
    {
      screen::pointer scrn(_screen.lock());                  dsI(scrn);
      display::pointer disp(scrn->get_display());              dsI(disp);
      return disp->_p->_xdisplay;
    }

    bool window::IMPL::get_visual_info( const screen::pointer & scrn )
    {
      display::pointer disp( scrn->get_display() );             dsI( disp );
      Display * xdisp = disp->_p->_xdisplay;

      VisualID vid = 0;
      if (vid) {
        XVisualInfo *vi, temp;
        int n;
        std::memset( &temp, 0, sizeof(temp) );
        temp.visualid = vid;
        if (vi = XGetVisualInfo(xdisp, VisualIDMask, &temp, &n)) {
          _vi = *vi;
          XFree(vi);
          return false;
        }
      }

      int screen = scrn->number();
      int depth = scrn->depth();

      dsL("screen depth: "<<depth);

      bool useDirectColorVisual = false;
      if ((useDirectColorVisual &&
           XMatchVisualInfo(xdisp, screen, depth, DirectColor, &_vi)) ||
          XMatchVisualInfo(xdisp, screen, depth, TrueColor, &_vi) ||
          XMatchVisualInfo(xdisp, screen, depth, PseudoColor, &_vi) ||
          XMatchVisualInfo(xdisp, screen, depth, StaticColor, &_vi) )
        return true;

      std::memset( &_vi, 0, sizeof(_vi) );
      return false;
    }

    bool window::IMPL::create_image_if_needed( int w, int h )
    {
      if ( _image.width() == w && _image.height() == h )
        return ( _ximage != NULL );

      _image.create(w, h, ds::graphics::image::ARGB_8888_PIXEL);
      dsI( _image.is_valid() );
      dsI( _image.width() == w );
      dsI( _image.height() == h );
      dsI( _image.pixels() != NULL );
      dsI( _image.pixel_type() == ds::graphics::image::ARGB_8888_PIXEL );
      dsI( _image.pixel_size() == 4 );
      
      if ( _ximage != NULL ) {
        _ximage->data = NULL;
        XDestroyImage( _ximage );
        _ximage = NULL;
      }

      Display * xdisp = x_display();

      dsI( xdisp != NULL );
      dsI( _vi.visual != NULL );
      dsI( 0 < _vi.depth );

      _ximage_pixels = malloc( h * w * (_vi.depth / 8) );
      _ximage = XCreateImage( xdisp, _vi.visual, _vi.depth, ZPixmap, 0,
                              reinterpret_cast<char*>(_ximage_pixels),//( _image.pixels() ),
                              _image.width(), _image.height(),
                              _image.pixel_size() * 8,/* bitmap pad */
                              _image.pixel_size() * w /* row bytes */ );

      if (_ximage==NULL) {
        free(_ximage_pixels);
        _ximage_pixels = NULL;
      } else {
        dsI( _image.width() == _ximage->width );
        dsI( _image.height() == _ximage->height );
      }

      dsL("image: bitmap_unit = "<<_ximage->bitmap_unit);
      dsL("image: bitmap_pad = "<<_ximage->bitmap_pad);
      dsL("image: depth = "<<_ximage->depth);
      dsL("image: bytes_per_line = "<<_ximage->bytes_per_line);
      dsL("image: bits_per_pixel = "<<_ximage->bits_per_pixel);
      dsL("image: red_mask = "<<_ximage->red_mask);
      dsL("image: green_mask = "<<_ximage->green_mask);
      dsL("image: blue_mask = "<<_ximage->blue_mask);

      return ( _ximage != NULL );
    }

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

      uint32_t ncolors = _vi.colormap_size;
      uint32_t bpp = _vi.depth / 8;

      dsL("rmask: "<<rmask);
      dsL("gmask: "<<gmask);
      dsL("bmask: "<<bmask);
      dsL("rshift: "<<rshift);
      dsL("gshift: "<<gshift);
      dsL("bshift: "<<bshift);
      dsL("colormap-size: "<<ncolors<<", "<<_vi.visual->map_entries);

      s += y * _image.width() * _image.pixel_size();
      d += y * _image.width() * bpp;

      int red, green, blue, pixel, n, c;
      for (n = 0; n < h; ++n) {
        s += x * _image.pixel_size();
        d += x * bpp;
        for (c = 0; c < w; ++c) {
          red = *(s + 0), green = *(s + 1), blue = *(s + 2);

          /*
          red   = (red   & rmask) << rshift;
          green = (green & gmask) << gshift;
          blue  = (blue  & bmask) << bshift;
          */
          red   = (red   >> rshift) & rmask;
          green = (green >> gshift) & gmask;
          blue  = (blue  >> bshift) & bmask;

          pixel = red | green | blue;

          *reinterpret_cast<short*>(d) = static_cast<short>( pixel & 0xFFFF );

          s += _image.pixel_size();
          d += bpp;
        }
        s += (_image.width() - w) * _image.pixel_size();
        d += (_image.width() - w) * bpp;
      }
    }

    void window::IMPL::create( const window::pointer & win )
    {
      dsI( !_native_win );

      screen::pointer scrn( _screen.lock() );           dsI( scrn );
      display::pointer disp( scrn->get_display());           dsI( disp );

      int x(0), y(0), w(400), h(300), bw(0);

      bool isVisualOK = get_visual_info( scrn );

      dsLif("cannot get visual", !isVisualOK);

      unsigned fc = scrn->black_pixel();
      unsigned bc = scrn->white_pixel();
 
      window::pointer root = scrn->root();           dsI(root);

      Window pr = root->_p->_native_win;
      register Display * const xdisp( disp->_p->_xdisplay );

      //XCreateWindow(...);
      _native_win = XCreateSimpleWindow( xdisp, pr, x, y, w, h, bw, fc, bc );
      dsI( _native_win );

      XGCValues gcv;
      gcv.graphics_exposures = False;
      _native_gc = XCreateGC( xdisp, _native_win, GCGraphicsExposures, &gcv );

      disp->_p->_winmap.insert( std::make_pair( _native_win, win ) );

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
    }

    void window::IMPL::destroy()
    {
      if ( _native_win ) {
        screen::pointer scrn( _screen.lock() );         dsI( scrn );
        display::pointer disp( scrn->get_display() );           dsI( disp );
        XDestroyWindow( disp->_p->_xdisplay, _native_win );
      }
    }

    void window::IMPL::select_input(long mask)
    {
      Display * xdisp  = x_display();
      dsI( xdisp );
      XSelectInput( xdisp, _native_win, mask );
    }

    ds::graphics::box window::IMPL::get_rect() const
    {
      XWindowAttributes a;
      std::memset( &a, 0, sizeof(a) );

      XGetWindowAttributes( x_display(), _native_win, &a );

      return boost::geometry::make<graphics::box>( a.x, a.y, a.width, a.height );
    }

    ds::graphics::image * window::IMPL::get_image_for_render()
    {
      const graphics::box wr( this->get_rect() );
      if ( wr.is_empty() ) {
        dsE("empty window: "<<_native_win);
        return NULL;
      }

      if (!create_image_if_needed(wr.width(), wr.height())) {
        dsE("Failed to create renderring buffer");
        return NULL;
      }

      dsI( _image.is_valid() );
      dsI( 0 < _image.width() );
      dsI( 0 < _image.height() );
      dsI( wr.width() == _image.width() );
      dsI( wr.height() == _image.height() );
      dsI( _ximage );

      return &_image;
    }

    bool window::IMPL::commit_image( const ds::graphics::box & dr )
    {
      convert_pixels( dr.x(), dr.y(), dr.width(), dr.height() );

      int copyCount = 0;
      Display * xdisp = x_display();

      if ( _dirty_rects.empty() ) {
        copyCount = 1;
        XPutImage( xdisp, _native_win, _native_gc, _ximage,
                   dr.x(), dr.y(), dr.x(), dr.y(), dr.width(), dr.height() );
      } else {
        ds::graphics::box r;
        __gnu_cxx::slist<ds::graphics::box>::const_iterator it;
        for (it = _dirty_rects.begin(); it != _dirty_rects.end(); ++it) {
          if ( (r = it->intersect(dr)).is_empty() )
            continue;

          ++copyCount;
          /*
            XCopyArea( _display->_xdisplay, _drawable, _native_win, _native_gc,
            r.x, r.y, r.w, r.h,
            r.x, r.y );
          */
          XPutImage( xdisp, _native_win, _native_gc, _ximage,
                     r.x(), r.y(), r.x(), r.y(), r.width(), r.height() );
        }
        _dirty_rects.clear();
      }

      if (0 < copyCount) {
        //XFlushGC( xdisp, _p->_native_gc );
        XSync( xdisp, False );
      }

      return false;
    }

  }// namespace ui
}//namespace ds
