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

      Atom WM_DELETE_WINDOW;

      IMPL()
        : xDisplay( NULL )
        , screen( -1 ) // TODO: should be '-1' ?
        , WM_DELETE_WINDOW( 0 )
      {
      }

      void init_atoms();

      bool pending();
      void pump_events();
      void dispatch( XEvent * event );
    };//struct display::IMPL

  }//namespace ui
}//namespace ds
