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
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "window_impl.h"
#include "display_impl.h"
#include <ds/debug.hpp>

namespace ds { namespace ui {

    bool window::IMPL::get_visual_info( const screen::pointer & scrn )
    {
      Display * xdisp = _disp->_p->_xdisp;

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
      if ( _image.width() == w &&
           _image.height() == h )
        return ( _ximage != NULL );

      _image.create(w, h, ds::graphics::image::ARGB_8888_PIXEL);
      dsI( _image.width() == w );
      dsI( _image.height() == h );
      dsI( _image.pixels() != NULL );
      
      if ( _ximage != NULL ) {
        _ximage->data = NULL;
        XDestroyImage( _ximage );
        _ximage = NULL;
      }

      int pixelBytes = 4;
      int rowBytes = pixelBytes * w;

      Display * xdisp = _disp->_p->_xdisp;

      dsI( xdisp != NULL );
      dsI( _vi.visual != NULL );
      dsI( 0 < _vi.depth );

      _ximage = XCreateImage(xdisp, _vi.visual, _vi.depth, ZPixmap, 0,
                             (char*)_image.pixels(),
                             _image.width(), _image.height(),
                             pixelBytes * 8, rowBytes );
      
      return ( _ximage != NULL );
    }

    void window::IMPL::create( const window::pointer & win )
    {
      dsI( !_xwin );
      dsI( _disp );

      int x(0), y(0), w(400), h(300), bw(0);

      screen::pointer scrn = _disp->default_screen();
      bool isVisualOK = get_visual_info( scrn );

      dsLif("cannot get visual", !isVisualOK);

      unsigned fc = scrn->black_pixel();
      unsigned bc = scrn->white_pixel();

      window::pointer root = scrn->root();
      dsI(root);

      Window pr = root->_p->_xwin;
      register Display * const xdisp( _disp->_p->_xdisp );

      //XCreateWindow(...);
      _xwin = XCreateSimpleWindow( xdisp, pr, x, y, w, h, bw, fc, bc );
      dsI( _xwin );

      XGCValues gcv;
      gcv.graphics_exposures = False;
      _gc = XCreateGC( xdisp, _xwin, GCGraphicsExposures, &gcv );

      _disp->_p->_winmap.insert( std::make_pair( _xwin, win ) );

      /* Allow window to be deleted by the window manager */
      XSetWMProtocols( xdisp, _xwin, &_disp->_p->WM_DELETE_WINDOW, 1 );

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
        XDestroyWindow( _disp->_p->_xdisp, _xwin );
      }
    }

    window::IMPL::~IMPL()
    {
      if (_ximage) {
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
      : _p( new IMPL(disp.get()) )
    {
      _p->create( this );
      disp->map( this );
      dsL4("window: "<<this<<"->"<<_p->_xwin);
    }

    window::~window()
    {
      dsL4("window: "<<this<<"->"<<_p->_xwin);

      _p->destroy();
      _p->_disp = NULL;
      delete _p;
    }

    display::pointer window::get_display() const
    {
      return _p->_disp;
    }

    void window::select_input(long mask)
    {
      XSelectInput( _p->_disp->_p->_xdisp, _p->_xwin, mask );
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

    graphics::irect window::rect() const
    {
      XWindowAttributes a;
      std::memset( &a, 0, sizeof(a) );
      XGetWindowAttributes( _p->_disp->_p->_xdisp, _p->_xwin, &a );
      graphics::irect r;
      r.set_xywh( a.x, a.y, a.width, a.height );
      return r;
    }

    void window::on_exposed( const event::window::exposed & a )
    {
      graphics::irect const wr( this->rect() );
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

      ds::graphics::irect dr;
      dr.set_xywh( a.x(), a.y(), a.width(), a.height() );
      if ( dr.is_empty() ) {
        dsE("empty dirty rect");
        return;
      }

      ds::graphics::canvas canvas( _p->_image );
      canvas.clip( dr );

      this->on_render( canvas );

      int copyCount = 0;
      Display * xdisp = _p->_disp->_p->_xdisp;

      if ( _p->_dirtyRects.empty() ) {
        copyCount = 1;
        XPutImage( xdisp, _p->_xwin, _p->_gc, _p->_ximage,
                   dr.left, dr.top, dr.left, dr.top, dr.width(), dr.height() );
      } else {
        ds::graphics::irect r;
        __gnu_cxx::slist<ds::graphics::irect>::const_iterator it;
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
                     r.left, r.top, r.left, r.top, r.width(), r.height() );
        }
        _p->_dirtyRects.clear();
      }

      if (0 < copyCount) {
        //XFlushGC( _p->_gc );
        XSync( xdisp, False );
      }
    }

  }// namespace ui
}//namespace ds
