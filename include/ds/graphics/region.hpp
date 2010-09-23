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
#       include <iosfwd>
#       include <ds/dso_visibility.hpp>

namespace ds { namespace graphics {
    
    struct DSO_PUBLIC region
    {
      region();
      region( const region & );
      explicit region( const box & );

      region & operator |= ( const box & );
      region & operator &= ( const box & );
      region & operator -= ( const box & );

      const region operator | ( const box & ) const;
      const region operator & ( const box & ) const;
      const region operator - ( const box & ) const;

      region & operator |= ( const region & );
      region & operator &= ( const region & );
      region & operator -= ( const region & );

      const region operator | ( const region & ) const;
      const region operator & ( const region & ) const;
      const region operator - ( const region & ) const;

      void dump( std::ostream & );

    private:
      class rasterizer;
      class operation;

      box _bounds;
      std::vector<box> _boxes;
    };//struct region
    
  }//namespace graphics
}//namespace ds

#endif//__DS_GRAPHICS_REGION__hpp____by_Duzy_Chan__
