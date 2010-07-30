/**                                                                -*- c++ -*-
 *    Copyright 2009-03-22 DuzySoft.com, by Zhan Xin-Ming£¨Õ²ÐÀÃú£©
 *    All rights reserved by Zhan Xin-Ming£¨Õ²ÐÀÃú£©
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id: image.ipp 801 2009-09-15 14:02:44Z duzy $
 *
 **/

#include <string>
#include <ds/graphics/image.hpp>
#include <ds/graphics/detail/image.ipp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/predicate.hpp>

namespace ds { namespace graphics {

    image::image()
      : _d( new detail::image )
    {
    }

    image::~image()
    {
      delete _d;
    }

    int image::width() const
    {
      return _d->width();
    }

    int image::height() const
    {
      return _d->height();
    }

    bool image::load( const std::string & file )
    {
      //TODO: using boost::filesystem::path?

      if ( file.empty() ) {
        dsD("empty image file name");
        return false;
      }

      if ( file[0] == ':' ) {
        TODO("load image from ds::resource");
        return false;
      }

      if ( 5 < file.size() ) {
        using boost::to_lower;
        using boost::iends_with;
        std::string suffix( file.substr(file.size()-5, std::string::npos) );
        to_lower( suffix );
        if ( iends_with(suffix, ".png") ) return _d->load_png( file );
        if ( iends_with(suffix, ".jpg") ) return _d->load_jpeg( file );
        if ( iends_with(suffix, ".jpeg") ) return _d->load_jpeg( file );
        if ( iends_with(suffix, ".tiff") ) return _d->load_tiff( file );
        if ( iends_with(suffix, ".skin") ) return _d->load_png( file );
      }
      return false;
    }

  }//namespace graphics
}//namespace ds
