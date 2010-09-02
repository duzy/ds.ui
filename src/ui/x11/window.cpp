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
#include <boost/geometry/algorithms/make.hpp>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "window_impl.h"
#include "display_impl.h"
#include <ds/debug.hpp>

namespace ds { namespace ui {

    bool window::IMPL::get_visual_info( const screen::pointer & scrn )
    {
      display::pointer disp( _disp.lock() );
      dsI( disp );

      Display * xdisp = disp->_p->_xdisp;

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

      display::pointer disp( _disp.lock() );
      dsI(disp);

      Display * xdisp = disp->_p->_xdisp;

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

    void window::IMPL::create( const window::pointer & win )
    {
      dsI( !_xwin );

      display::pointer disp( _disp.lock() );
      dsI( disp );

      int x(0), y(0), w(400), h(300), bw(0);

      screen::pointer scrn = disp->default_screen();
      bool isVisualOK = get_visual_info( scrn );

      dsLif("cannot get visual", !isVisualOK);

      unsigned fc = scrn->black_pixel();
      unsigned bc = scrn->white_pixel();
 
      window::pointer root = scrn->root();
      dsI(root);

      Window pr = root->_p->_xwin;
      register Display * const xdisp( disp->_p->_xdisp );

      //XCreateWindow(...);
      _xwin = XCreateSimpleWindow( xdisp, pr, x, y, w, h, bw, fc, bc );
      dsI( _xwin );

      XGCValues gcv;
      gcv.graphics_exposures = False;
      _gc = XCreateGC( xdisp, _xwin, GCGraphicsExposures, &gcv );

      disp->_p->_winmap.insert( std::make_pair( _xwin, win ) );

      /* Allow window to be deleted by the window manager */
      XSetWMProtocols( xdisp, _xwin, &disp->_p->WM_DELETE_WINDOW, 1 );

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
      XSelectInput( xdisp, _xwin, eventMask );
    }

    void window::IMPL::destroy()
    {
      if ( _xwin ) {
        display::pointer disp( _disp.lock() );          dsI(disp);
        XDestroyWindow( disp->_p->_xdisp, _xwin );
      }
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

    //////////////////////////////////////////////////////////////////////
    
    window::window()
      : _p( new IMPL(NULL) )
    {
      dsL4("window: "<<this<<"->"<<_p->_xwin);
    }

    window::window( const display::pointer & disp )
      : _p( new IMPL(disp/*.get()*/) )
    {
      _p->create( this );
      disp->map( this );
      dsL4("window: "<<this<<"->"<<_p->_xwin);
    }

    window::~window()
    {
      dsL4("window: "<<this<<"->"<<_p->_xwin);

      _p->destroy();
      _p->_disp.reset();// = NULL;
      delete _p;
    }

    display::pointer window::get_display() const
    {
      return _p->_disp.lock();
    }

    void window::select_input(long mask)
    {
      display::pointer disp(_p->_disp.lock());                  dsI(disp);
      XSelectInput( disp->_p->_xdisp, _p->_xwin, mask );
    }

    void window::destroy()
    {
      _p->destroy();
    }

    void window::show()
    {
      // TODO: ...
    }

    void window::hide()
    {
      // TODO: ...
    }

    void window::move( int x, int y )
    {
      // TODO: ...
    }

    void window::resize( int w, int h )
    {
      // TODO: ...
    }

    graphics::box window::rect() const
    {
      XWindowAttributes a;
      std::memset( &a, 0, sizeof(a) );

      display::pointer disp(_p->_disp.lock());          dsI(disp);
      XGetWindowAttributes( disp->_p->_xdisp, _p->_xwin, &a );
      return boost::geometry::make<graphics::box>( a.x, a.y, a.width, a.height );
    }

    void window::on_exposed( const event::window::exposed & a )
    {
      graphics::box const wr( this->rect() );
      if ( wr.is_empty() ) {
        dsE("empty window: "<<_p->_xwin);
        return;
      }

      if (!_p->create_image_if_needed(wr.width(), wr.height())) {
        dsE("Failed to create renderring buffer");
        return;
      }
      dsI( _p->_image.is_valid() );
      dsI( 0 < _p->_image.width() );
      dsI( 0 < _p->_image.height() );
      dsI( wr.width() == _p->_image.width() );
      dsI( wr.height() == _p->_image.height() );
      dsI( _p->_ximage );

      ds::graphics::box dr( boost::geometry::make<graphics::box>( a.x(), a.y(), a.width(), a.height() ) );
      if ( dr.is_empty() ) {
        dsE("empty dirty rect");
        return;
      }

      ds::graphics::canvas canvas( _p->_image );
      canvas.clip( dr );

      this->on_render( canvas );

      _p->convert_pixels( a.x(), a.y(), a.width(), a.height() );

      int copyCount = 0;
      display::pointer disp(_p->_disp.lock());          dsI(disp);
      Display * xdisp = disp->_p->_xdisp;

      if ( _p->_dirtyRects.empty() ) {
        copyCount = 1;
        XPutImage( xdisp, _p->_xwin, _p->_gc, _p->_ximage,
                   dr.x(), dr.y(), dr.x(), dr.y(), dr.width(), dr.height() );
      } else {
        ds::graphics::box r;
        __gnu_cxx::slist<ds::graphics::box>::const_iterator it;
        for (it = _p->_dirtyRects.begin(); it != _p->_dirtyRects.end(); ++it) {
          if ( (r = it->intersect(dr)).is_empty() )
            continue;

          ++copyCount;
          /*
            XCopyArea( _disp->_p->_xdisp, _p->_drawable, _p->_xwin, _p->_gc,
            r.x, r.y, r.w, r.h,
            r.x, r.y );
          */
          XPutImage( xdisp, _p->_xwin, _p->_gc, _p->_ximage,
                     r.x(), r.y(), r.x(), r.y(), r.width(), r.height() );
        }
        _p->_dirtyRects.clear();
      }

      if (0 < copyCount) {
        //XFlushGC( xdisp, _p->_gc );
        XSync( xdisp, False );
      }
    }

  }// namespace ui
}//namespace ds
