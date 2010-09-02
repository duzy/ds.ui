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

      display::weak_ref _disp; // Never hold a display::pointer here(cycle leak)!

      window::pointer _root;

      IMPL()
        : _xscrn( NULL )
        , _disp( NULL )
        , _root( NULL )
      {
      }
    };//struct screen::IMPL
    
  }//namespace ui
}//namespace ds
