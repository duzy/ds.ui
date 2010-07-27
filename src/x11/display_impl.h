/**
 *    Copyright 2010-07-25 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

namespace ds { namespace ui {

    struct display::IMPL
    {
      Display * xDisplay;
      int screen;

      window::pointer_t root;

      IMPL()
        : xDisplay( NULL )
        , screen( -1 ) // TODO: is '-1' ?
        , root( NULL )
      {
      }

      void loop();
      void dispatch( XEvent * event );
    };//struct display::IMPL

  }//namespace ui
}//namespace ds
