/**
 *    Copyright 2010-08-17 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#       include <ds/dso_visibility.hpp>

namespace ds { namespace graphics {
    class canvas;    
  }//namespace graphics
}//namespace ds

namespace ds { namespace ui {

    class DSO_PUBLIC view
    {
    protected:
      virtual void on_render( ds::graphics::canvas & ) = 0;
    };//class view
    
  }//namespace ui
}//namespace ds
