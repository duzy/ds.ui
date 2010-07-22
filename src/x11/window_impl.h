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

      display::pointer_t disp;

      IMPL( const display::pointer_t & d )
        : xWindow( NULL )
        , disp( d )
      {
      }
    };//struct window::IMPL
    
  }//namespace ui
}//namespace ds
