/**								-*- c++ -*-
 *    Copyright 2009-03-22 DuzySoft.com, by Duzy Chan£¨Õ²ÐÀÃú£©
 *    All rights reserved by Duzy Chan£¨Õ²ÐÀÃú£©
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id: graphics_image.cpp 614 2009-05-12 03:23:30Z duzy $
 *
 **/

#include <ds/debug.hpp>
#include <ds/graphics/gil/image.hpp>
#include <boost/gil/image.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>
//#include <boost/gil/extension/io/jpeg_dynamic_io.hpp>
//#include <boost/gil/extension/io/tiff_dynamic_io.hpp>

namespace ds { namespace graphics { namespace gil {

      image::image() : any_image_t()
      {
      }

      bool image::load_png( const std::string & file )
      {
        try {
          //png_read_image( file, *reinterpret_cast<any_image_t*>(this) );
          //png_read_image( file, *this );
          png_read_image( file, any() );
          return true;
        }
        catch( std::exception const & e ) {
          dsE("can't load PNG image: "<<file<<"("<<e.what()<<")");
        }
        catch( ... ) {
          dsE("can't load PNG image: "<<file);
        }
        return false;
      }

      bool image::load_jpeg( const std::string & file )
      {
        TODO("load JPEG iamge");
        return false;
      }

      bool image::load_tiff( const std::string & file )
      {
        TODO("load TIFF image");
        return false;
      }

    }//namespace gil
  }//namespace graphics
}//namespace ds
