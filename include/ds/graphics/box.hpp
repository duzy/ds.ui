/**
 *    Copyright 2010-08-22 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#ifndef __DS_GRAPHICS_BOX__hpp____by_Duzy_Chan__
#define __DS_GRAPHICS_BOX__hpp____by_Duzy_Chan__ 1
#       include "point.hpp"
#       include <boost/geometry/box.hpp>

namespace ds { namespace graphics {
    
    typedef boost::geometry::box<point> box;
    
  }//namespace graphics
}//namespace ds

#endif//__DS_GRAPHICS_BOX__hpp____by_Duzy_Chan__
