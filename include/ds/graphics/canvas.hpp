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
    struct color;
    struct point;
    struct segment;
    struct line;
    struct polygon;
    struct box;
    struct linestring;

    class canvas : boost::noncopyable
    {
    public:
      canvas( image & );

      bool clip( const box & );
      bool clip( const polygon & );

      void render( const color & );
      void render( const image & );
      void render( const polygon & );
      void render( const box & );

      void stroke( const point & );
      void stroke( const segment & );
      void stroke( const line & );
      void stroke( const polygon & );
      void stroke( const box & );

    private:
      image * _image;
    };//class canvas
    
  }//namespace graphics
}//namespace ds

#endif//__DS_GRAPHICS_CANVAS_hpp____by_Duzy_Chan__
