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
      Window xWindow;

      display::pointer_t disp; // TODO: a window should not keep a display::pointer_t

      // TODO: should keep a reference to the screen

      IMPL( const display::pointer_t & d )
        : xWindow( NULL )
        , disp( d )
      {
      }

      void create();
      void destroy();
    };//struct window::IMPL
    
  }//namespace ui
}//namespace ds
