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
      Screen *_xscrn;

      display * _disp; // Never hold a display::pointer_t here(cycle leak)!

      window::pointer_t _root;

      IMPL()
        : _xscrn( NULL )
        , _disp( NULL )
        , _root( NULL )
      {
      }
    };//struct screen::IMPL
    
  }//namespace ui
}//namespace ds
