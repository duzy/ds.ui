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
    {
    }

    screen::IMPL::~IMPL()
    {
      if (_root) {
        _root->_p->_native_win = NULL; // prevent destroy native root win
        _root.reset( NULL );
      }
    }

    window::pointer screen::IMPL::get_root_win( const screen::pointer & s ) const
    {
      if ( !_root ) {
        screen::IMPL * that = const_cast<screen::IMPL*>( this );
        that->_root.reset( new window );
        that->_root->_p->_screen = s;
        that->_root->_p->_native_win = NULL; //XRootWindowOfScreen( _xscrn );
      }
      return _root;
    }

    int screen::IMPL::number() const
    {
      return 0;
    }

    int screen::IMPL::width() const
    {
      return -1;//XWidthOfScreen( _xscrn );
    }

    int screen::IMPL::height() const
    {
      return -1;//XHeightOfScreen( _xscrn );
    }

    unsigned screen::IMPL::black_pixel() const
    {
      return 0x00000000;//XBlackPixelOfScreen( _xscrn );
    }

    unsigned screen::IMPL::white_pixel() const
    {
      return 0xFFFFFFFF;//XWhitePixelOfScreen( _xscrn );
    }

    int screen::IMPL::depth() const
    {
      return 24;//XDefaultDepthOfScreen( _xscrn );
    }
    
  }//namespace ui
}//namespace ds
