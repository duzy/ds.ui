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
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "screen_impl.h"
#include "display_impl.h"
#include "window_impl.h"
#include <ds/debug.hpp>

namespace ds { namespace ui {

    screen::screen()
      : _p( new IMPL )
    {
      dsL("screen: "<<this<<"->"<<_p->_xscrn);
    }

    screen::~screen()
    {
      dsL("screen: "<<this<<"->"<<_p->_xscrn);
      delete _p;
    }

    shared_object<display>::pointer screen::get_display() const
    {
      return _p->_display.lock();
    }

    window::pointer screen::root() const
    {
      if ( !_p->_root ) {
        _p->_root.reset( new window );
        _p->_root->_p->_screen = pointer(const_cast<screen*>(this));
        _p->_root->_p->_xwin = XRootWindowOfScreen( _p->_xscrn );
      }
      return _p->_root;
    }

    int screen::number() const
    {
      return XScreenNumberOfScreen( _p->_xscrn );
    }

    int screen::width() const
    {
      return XWidthOfScreen( _p->_xscrn );
    }

    int screen::height() const
    {
      return XHeightOfScreen( _p->_xscrn );
    }

    unsigned screen::black_pixel() const
    {
      return XBlackPixelOfScreen( _p->_xscrn );
    }

    unsigned screen::white_pixel() const
    {
      return XWhitePixelOfScreen( _p->_xscrn );
    }

    int screen::depth() const
    {
      return XDefaultDepthOfScreen( _p->_xscrn );
    }
    
  }//namespace ui
}//namespace ds
