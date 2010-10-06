/**
 *    Copyright 2010-08-17 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#ifndef __DS_UI_VIEW__hpp____by_Duzy_Chan__
#define __DS_UI_VIEW__hpp____by_Duzy_Chan__ 1
#       include "../visibility.h"

namespace ds {
  namespace graphics {
    class canvas;    
  }//namespace graphics

  namespace ui {

    struct DS_UI_PUBLIC view
    {
    protected:
      virtual void on_render( ds::graphics::canvas & ) = 0;
    };//struct view
    
  }//namespace ui
}//namespace ds

#endif//__DS_UI_VIEW__hpp____by_Duzy_Chan__
