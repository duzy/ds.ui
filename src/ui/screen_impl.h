/**
 *    Copyright 2010-07-26 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

namespace ds { namespace ui {

    struct screen::IMPL
    {
      IMPL();
      ~IMPL();

      const display::weak_ref & get_display_ref() const { return _display; }

      window::pointer get_root_win( const screen::pointer & ) const;
      int number() const;
      int width() const;
      int height() const;
      unsigned black_pixel() const;
      unsigned white_pixel() const;
      int depth() const;

      display::weak_ref _display;
      window::pointer _root; // TODO: avoid using window::pointer ??

#ifdef _WIN32
#elif defined(X11)
      XVisualInfo _vi;
      Screen * _xscrn;

      XVisualInfo * get_visual_info();
#else
#   error unsupported platform
#endif
    };//struct screen::IMPL
    
  }//namespace ui
}//namespace ds
