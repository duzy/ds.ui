/**
 *    Copyright 2010-07-25 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

namespace ds { namespace ui {
    
    struct window::IMPL
    {
      display * _disp; // Never hold a display::pointer_t(will leak)!

      Window _xwin; // Window and Pixmap are Drawable

      XImage _image; 
      Drawable _drawable; // Pixmap, Window, etc...
      GC _gc;

      std::slist<ds::graphics::rect> _dirtyRects;

      IMPL( display * d )
        : _disp( d )
        , _xwin( NULL )
        , _drawable( NULL )
        , _gc( NULL )
        , _dirtyRects()
      {
      }

      void create( const window::pointer & );
      void destroy();
    };//struct window::IMPL
    
  }//namespace ui
}//namespace ds
