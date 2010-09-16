/**
 *    Copyright 2010-07-26 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/ui/screen.hpp>
#include <ds/ui/display.hpp>
#include <ds/ui/window.hpp>
#include <ds/debug.hpp>
#include "../window_impl.h"
#include "../screen_impl.h"
#include "../display_impl.h"

namespace ds { namespace ui {

    screen::IMPL::IMPL()
      : _display( NULL )
      , _root( NULL )
      , _xscrn( NULL )
    {
    }

    screen::IMPL::~IMPL()
    {
      if (_root) {
        _root->_p->_native_win = NULL; // prevent destroy native root win
        _root.reset( NULL );
      }
    }

    XVisualInfo * screen::IMPL::get_visual_info()
    {
      if (_vi.visual /* TODO: check _vi.visualid */ )
        return &_vi;

      display::pointer disp( _display.lock() );                 dsI( disp );
      Display * xdisp = disp->_p->_xdisplay;                    dsI( xdisp);

      VisualID vid = 0;
      if (vid) {
        XVisualInfo *vi, temp;
        int n;
        std::memset( &temp, 0, sizeof(temp) );
        temp.visualid = vid;
        if (vi = XGetVisualInfo(xdisp, VisualIDMask, &temp, &n)) {
          _vi = *vi;
          XFree(vi);
          return NULL;
        }
      }

      int screen = this->number();
      int depth = this->depth();

      dsL("screen depth: "<<depth);

      bool useDirectColorVisual = false;
      if ((useDirectColorVisual &&
           XMatchVisualInfo(xdisp, screen, depth, DirectColor, &_vi)) ||
          XMatchVisualInfo(xdisp, screen, depth, TrueColor, &_vi) ||
          XMatchVisualInfo(xdisp, screen, depth, PseudoColor, &_vi) ||
          XMatchVisualInfo(xdisp, screen, depth, StaticColor, &_vi) )
        return &_vi;

      std::memset( &_vi, 0, sizeof(_vi) );
      return NULL;
    }

    window::pointer screen::IMPL::get_root_win( const screen::pointer & s ) const
    {
      if ( !_root ) {
        screen::IMPL * that = const_cast<screen::IMPL*>( this );
        that->_root.reset( new window );
        that->_root->_p->_screen = s;
        that->_root->_p->_native_win = XRootWindowOfScreen( _xscrn );
      }
      return _root;
    }

    int screen::IMPL::number() const
    {
      return XScreenNumberOfScreen( _xscrn );
    }

    int screen::IMPL::width() const
    {
      return XWidthOfScreen( _xscrn );
    }

    int screen::IMPL::height() const
    {
      return XHeightOfScreen( _xscrn );
    }

    unsigned screen::IMPL::black_pixel() const
    {
      return XBlackPixelOfScreen( _xscrn );
    }

    unsigned screen::IMPL::white_pixel() const
    {
      return XWhitePixelOfScreen( _xscrn );
    }

    int screen::IMPL::depth() const
    {
      return XDefaultDepthOfScreen( _xscrn );
    }
    
  }//namespace ui
}//namespace ds
