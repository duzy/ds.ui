/**
 *    Copyright 2010-08-22 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#ifndef __DS_GRAPHICS_SEGMENT__hpp____by_Duzy_Chan__
#define __DS_GRAPHICS_SEGMENT__hpp____by_Duzy_Chan__ 1
#       include "point.hpp"
#       include <boost/geometry/segment.hpp>

namespace ds { namespace graphics {

    typedef boost::geometry::segment<point> segment;
    
  }//namespace graphics
}//namespace ds

#endif//__DS_GRAPHICS_SEGMENT__hpp____by_Duzy_Chan__
