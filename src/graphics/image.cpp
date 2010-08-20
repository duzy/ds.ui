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
#include <ds/graphics/gil/image.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <ds/debug.hpp>

namespace ds { namespace graphics {

    image::image()
      : _isView( 0 )
      , _m( NULL )
    {
    }

    image::image( int w, int h, PixelType pt, uint8_t * data )
      : _isView( 1 )
      , _m( NULL )
    {
      switch (pt) {
      case RGB_565_PIXEL:
        //_v = new gil::view( w, h, (gil::abgr8_image_t::x_iterator*)data, rowBytes );
        break;
      case ARGB_8888_PIXEL:
        _v = new gil::view( w, h, (gil::abgr8_image_t::value_type*)data, w*4 );
        break;
      }
    }

    image::~image()
    {
      if (_isView) delete _v;
      else delete _m;
    }

    int image::width() const
    {
      return _isView ? _v->width() : _m->width();
    }

    int image::height() const
    {
      return _isView ? _v->height() : _m->height();
    }

    bool image::load( const std::string & file )
    {
      //TODO: using boost::filesystem::path?

      if ( file.empty() ) {
        dsE("empty image file name");
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
        if ( iends_with(suffix, ".png") ) return _m->load_png( file );
        if ( iends_with(suffix, ".jpg") ) return _m->load_jpeg( file );
        if ( iends_with(suffix, ".jpeg") ) return _m->load_jpeg( file );
        if ( iends_with(suffix, ".tiff") ) return _m->load_tiff( file );
        if ( iends_with(suffix, ".skin") ) return _m->load_png( file );
      }
      return false;
    }

  }//namespace graphics
}//namespace ds
