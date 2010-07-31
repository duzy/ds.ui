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
#       include <boost/noncopyable.hpp>

namespace ds { namespace graphics {

    struct image;
    struct color;

    struct point;
    struct segment;
    struct box;
    struct ring;
    struct polygon;
    struct linestring;

    class canvas;
    class paint
    {
      struct IMPL;

    public:
      paint();
      ~paint();

    private:
      IMPL * _imp;

      friend class canvas;
    };//class paint

    class canvas : boost::noncopyable
    {
      struct IMPL;

    public:
      canvas( image & );
      ~canvas();

      /*
      enum ClipOp {
        ClipDifference, //!< subtract the op region from the first region
        ClipIntersect,  //!< intersect the two regions
        ClipUnion,      //!< union (inclusive-or) the two regions
        ClipXOR,        //!< exclusive-or the two regions
        ClipReverseDifference, //!< subtract the first region from the op region
        ClipReplace     //!< replace the dst region with the op region
      };

      bool clip( const box &,           ClipOp = ClipIntersect );
      bool clip( const polygon &,       ClipOp = ClipIntersect );
      */
      bool clip( const box & );
      bool clip( const polygon & );

      struct clip_t
      {
        clip_t & operator  = ( const box & ); //!< replace
        clip_t & operator -= ( const box & ); //!< subtract
        //clip_t & operator += ( const box & ); //!< same as '|='
        clip_t & operator |= ( const box & ); //!< inclusive-or
        clip_t & operator ^= ( const box & ); //!< exclusive-or
        clip_t & intersect( const box & ); //!< intersect
      };

      /**
         myCanvas.clip() |= box1;
       */
      clip_t clip();

      void render( const color & );
      void render( const image & );

      void render( const point &,       const paint & );
      void render( const box &,         const paint & );
      void render( const polygon &,     const paint & );
      //void render( const ds::ustring &, const paint & );

      void stroke( const point &,       const paint & );
      void stroke( const segment &,     const paint & );
      void stroke( const box &,         const paint & );
      void stroke( const polygon &,     const paint & );
      //void stroke( const ds::ustring &, const paint & );

    private:
      IMPL * _imp;
    };//class canvas
    
  }//namespace graphics
}//namespace ds

#endif//__DS_GRAPHICS_CANVAS_hpp____by_Duzy_Chan__
