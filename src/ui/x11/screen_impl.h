/**
 *    Copyright 2010-07-26 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace ds { namespace ui {

    struct screen::IMPL
    {
      display::weak_ref _display;

      window::pointer _root; // TODO: avoid using window::pointer ??

      Screen * _xscrn;

      IMPL()
        : _display( NULL )
        , _root( NULL )
        , _xscrn( NULL )
      {
      }

      const display::weak_ref & get_display_ref() const { return _display; }

      window::pointer get_root_win( const screen::pointer & ) const;
      int number() const;
      int width() const;
      int height() const;
      unsigned black_pixel() const;
      unsigned white_pixel() const;
      int depth() const;
    };//struct screen::IMPL
    
  }//namespace ui
}//namespace ds
