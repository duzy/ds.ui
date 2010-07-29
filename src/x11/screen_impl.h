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
      Screen *xScreen;

      // TODO: keep a display::pointer_t here

      IMPL()
        : xScreen( NULL )
      {
      }
    };//struct screen::IMPL
    
  }//namespace ui
}//namespace ds
