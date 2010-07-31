/**
 *    Copyright 2010-07-30 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#ifndef __DS_GRAPHICS_COLOR__hpp____by_Duzy_Chan__
#define __DS_GRAPHICS_COLOR__hpp____by_Duzy_Chan__ 1

namespace ds { namespace graphics {

    struct color
    {
      uint8_t red;
      uint8_t green;
      uint8_t blue;
      uint8_t alpha;

      color( float a, float r, float g, float b )
        : red   ( uint32_t(0xFF * r) & 0xFF )
        , green ( uint32_t(0xFF * g) & 0xFF )
        , blue  ( uint32_t(0xFF * b) & 0xFF )
        , alpha ( uint32_t(0xFF * a) & 0xFF )
      {
      }
    };
    
  }//namespace graphics
}//namespace ds

#endif//__DS_GRAPHICS_COLOR__hpp____by_Duzy_Chan__
