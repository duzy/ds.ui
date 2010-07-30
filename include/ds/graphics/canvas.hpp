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
#       include <ds/graphics/rect.hpp>
#       include <boost/noncopyable.hpp>

namespace ds { namespace graphics {

    struct image;

    class canvas : boost::noncopyable
    {
    public:
      canvas( image & );

      void clip( const irect & );

      void render( ... );
      void stroke( ... );

    private:
      image * _image;
    };//class canvas
    
  }//namespace graphics
}//namespace ds

#endif//__DS_GRAPHICS_CANVAS_hpp____by_Duzy_Chan__
