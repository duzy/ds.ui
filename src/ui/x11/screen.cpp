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
      //display::pointer disp( new display ); // TODO: avoid making a new instance of the display
      //disp->_p->_xdisp = XDisplayOfScreen( _p->_xscrn );
      ////disp->_p->screen = XScreenNumberOfScreen( _p->_xscrn );
      //return disp;
      return _p->_disp; // implicitly conversion
    }

    window::pointer screen::root() const
    {
      if ( !_p->_root ) {
        _p->_root.reset( new window );
        _p->_root->_p->_disp = get_display().get();
        _p->_root->_p->_xwin = XRootWindowOfScreen( _p->_xscrn );
      }
      return _p->_root;
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
    
  }//namespace ui
}//namespace ds
