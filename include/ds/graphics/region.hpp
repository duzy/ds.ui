/**
 *    Copyright 2010-09-21 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#ifndef __DS_GRAPHICS_REGION__hpp____by_Duzy_Chan__
#define __DS_GRAPHICS_REGION__hpp____by_Duzy_Chan__ 1
#       include "box.hpp"
#       include <vector>

namespace ds { namespace graphics {
    
    struct region
    {
      region();

      region & operator |= ( const region & );
      region & operator &= ( const region & );
      region & operator -= ( const region & );

      const region operator |= ( const region & ) const;
      const region operator &= ( const region & ) const;
      const region operator -= ( const region & ) const;

    private:
      class rasterizer;

      box _bounds;
      std::vector<box> _boxes;
    };//struct region
    
  }//namespace graphics
}//namespace ds

#endif//__DS_GRAPHICS_REGION__hpp____by_Duzy_Chan__
