/**
 *    Copyright 2010-08-15 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/ui/window.hpp>
#include <ds/graphics/canvas.hpp>
#include <ds/debug.hpp>

namespace ds { namespace ui {

    void window::on_render( ds::graphics::canvas & canvas )
    {
      dsL("render: "<<this);
    }
    
  }//namespace ui
}//namespace ds
