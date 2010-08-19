/**
 *    Copyright 2010-08-15 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#ifndef __DS_GRAPHICS_CANVAS_hpp____by_Duzy_Chan__
#define __DS_GRAPHICS_CANVAS_hpp____by_Duzy_Chan__ 1

namespace ds { namespace graphics {

    namespace gil
    {
      struct image;
      struct view;
    }//namespace gil

    class canvas
    {
    public:
      canvas( gil::image & );
      canvas( gil::view & );

      void render( ... );
      void stroke( ... );

    private:
      unsigned _isView : 1;
      union {
        gil::image      * _d;
        gil::view       * _v;
      };
    };//class canvas
    
  }//namespace graphics
}//namespace ds

#endif//__DS_GRAPHICS_CANVAS_hpp____by_Duzy_Chan__
