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

    void window::IMPL::get_visual( const screen::pointer & scrn )
    {
      Display * xdisp = _disp->_p->_xdisp;

      VisualID vid = 0;
      if (vid) {
        XVisualInfo *vi, temp;
        int n;
        std::memset( &temp, 0, sizeof(temp) );
        temp.visualid = vid;
        if (vi = XGetVisualInfo(xdisp, VisualIDMask, &temp, &n)) {
          _visual = *vi;
          XFree(vi);
          return;
        }
      }

      int screen = scrn->number();
      int depth = scrn->depth();
      bool useDirectColorVisual = false;
      if ((useDirectColorVisual &&
           XMatchVisualInfo(xdisp, screen, depth, DirectColor, &_visual)) ||
          XMatchVisualInfo(xdisp, screen, depth, TrueColor, &_visual) ||
          XMatchVisualInfo(xdisp, screen, depth, PseudoColor, &_visual) ||
          XMatchVisualInfo(xdisp, screen, depth, StaticColor, &_visual) )
        return;

      std::memset( &_visual, 0, sizeof(_visual) );
    }

    void window::IMPL::create( const window::pointer & win )
    {
      dsI( !_xwin );
      dsI( _disp );

      int x(0), y(0), w(400), h(300), bw(0);

      screen::pointer scrn = _disp->default_screen();
      get_visual( scrn );

      unsigned fc = scrn->black_pixel();
      unsigned bc = scrn->white_pixel();

      window::pointer root = scrn->root();
      dsI(root);

      Window pr = root->_p->_xwin;
      register Display *xDisplay( _disp->_p->_xdisp );

      //XCreateWindow(...);
      _xwin = XCreateSimpleWindow( xDisplay, pr, x, y, w, h, bw, fc, bc );

      _disp->_p->_winmap.insert( std::make_pair( _xwin, win ) );

      /* Allow window to be deleted by the window manager */
      XSetWMProtocols( xDisplay, _xwin, &_disp->_p->WM_DELETE_WINDOW, 1 );

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
      XSelectInput( xDisplay, _xwin, eventMask );
    }

    void window::IMPL::destroy()
    {
      if ( _xwin ) {
        XDestroyWindow( _disp->_p->_xdisp, _xwin );
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
    }

    void window::hide()
    {
    }

    void window::move( int x, int y )
    {
    }

    void window::resize( int w, int h )
    {
    }

    void window::on_exposed( const event::window::exposed & a )
    {
      if (_p->_ximage.width <= 0 || _p->_ximage.height <= 0) {
        dsE("Invalid buffer image size");
        return;
      }
      if (_p->_ximage.data==NULL) {
        dsE("Uninitialized buffer image of size: "
            <<_p->_ximage.width<<"x"<<_p->_ximage.height);
        return;
      }

      ds::graphics::irect dr;
      dr.set_xywh( a.x(), a.y(), a.width(), a.height() );

      ds::graphics::image img( _p->_ximage.width,
                               _p->_ximage.height,
                               //_p->_ximage.bits_per_pixel,
                               ds::graphics::image::ARGB_8888_PIXEL,
                               (uint8_t*)_p->_ximage.data );

      ds::graphics::canvas canvas( img );
      canvas.clip( dr );

      this->on_render( canvas );

      Display * xdisp = _p->_disp->_p->_xdisp;

      int copyCount = 0;
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
        XPutImage( xdisp, _p->_xwin, _p->_gc, &_p->_ximage,
                   r.left, r.top, r.left, r.top, r.width(), r.height() );
      }

      _p->_dirtyRects.clear();

      if (0 < copyCount) {
        XSync( xdisp, False );
      }
    }

  }// namespace ui
}//namespace ds
