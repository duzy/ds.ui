/**
 *    Copyright 2009-03-22 DuzySoft.com, by Zhan Xin-Ming£¨Õ²ÐÀÃú£©
 *    All rights reserved by Zhan Xin-Ming£¨Õ²ÐÀÃú£©
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id: image.hpp 801 2009-09-15 14:02:44Z duzy $
 *
 **/

#ifndef __DS_GRAPHICS_IMAGE_HPP____by_Duzy_Chan__
#define __DS_GRAPHICS_IMAGE_HPP____by_Duzy_Chan__ 1

namespace ds { namespace graphics {

    namespace detail
    {
      struct image; //== Forward declaration ==
    }//namespace detail

    struct image
    {
      image();
      virtual ~image();

      /**
       *  @brief Load PNG image from a disk file or ds::resource.
       *
       *  @code
       *  image img1, img2;
       *  img1.load( "/images/foo.png" ); //!< load a disk file
       *  img2.load( ":/images/foo.png" ); //!< load from ds::resource
       *  @endcode
       */
      bool load( const std::string & file );

      int width() const;
      int height() const;

      #if 0
      enum resize_e { resize_smooth }
      void resize(  );
      #endif

    private:
      detail::image * _d;
    };//struct image

  }//namespace graphics
}//namespace ds


#endif//__DS_GRAPHICS_IMAGE_HPP____by_Duzy_Chan__
