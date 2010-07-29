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
      Window _xwin;

      display * _disp; // Never hold a display::pointer_t(will leak)!

      IMPL( display * d )
        : _xwin( NULL )
        , _disp( d )
      {
      }

      void create( const window::pointer & );
      void destroy();
    };//struct window::IMPL
    
  }//namespace ui
}//namespace ds
