/**                                                                -*- c++ -*-
 *    Copyright 2009-03-22 DuzySoft.com, by Zhan Xin-Ming£¨Õ²ÐÀÃú£©
 *    All rights reserved by Zhan Xin-Ming£¨Õ²ÐÀÃú£©
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id: image.ipp 801 2009-09-15 14:02:44Z duzy $
 *
 **/

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

    image::image( int w, int h, PixelType pt )
      : _isView( 0 )
      , _m( NULL )
    {
      bool ok = create(w, h, pt);
      dsI( ok );
    }

    image::image( int w, int h, PixelType pt, uint8_t * data )
      : _isView( 1 )
      , _v( NULL )
    {
      bool ok = create(w, h, pt, data);
      dsI( ok );
    }

    image::~image()
    {
      if ( _isView ) delete _v;
      else delete _m;
    }

    bool image::is_valid() const
    {
      return (_m != NULL) /*&& (_v != NULL)*/;
    }

    bool image::create( int w, int h, PixelType pt )
    {
      if ( _isView ) delete _v;
      else delete _m;

      _isView = 0;
      _m = NULL;

      switch ( pt ) {
      case RGB_565_PIXEL:
        //_m = new gil::image(gil::rgb565(w, h));
        break;
      case ARGB_8888_PIXEL:
        _m = new gil::image(gil::abgr8_image_t(w, h));
        break;
      }

      if ( _m ) {
        _m->recreate( w, h );
      }

      return ( _m != NULL );
    }

    bool image::create( int w, int h, PixelType pt, uint8_t * data )
    {
      if ( _isView ) delete _v;
      else delete _m;

      _isView = 1;
      _v = NULL;

      switch ( pt ) {
      case RGB_565_PIXEL:
        //_v = new gil::view( w, h, (gil::abgr8_image_t::x_iterator*)data, rowBytes );
        break;
      case ARGB_8888_PIXEL:
        _v = new gil::view( w, h, (gil::abgr8_image_t::value_type*)data, w*4 );
        break;
      }

      return ( _v != NULL );
    }

    /*
    const uint8_t * image::pixels() const
    {
      return const_cast<image*>(this)->pixels();
    }
    */

    struct get_image_pixels_f
    {
      typedef uint8_t * result_type;

      template<typename V>
      inline result_type operator()( const V & v ) const
      {
        return reinterpret_cast<result_type>(&(v[0]));
      }

      inline uint8_t * result_of_view( const gil::any_image_t::view_t & v ) const
      {
        return boost::gil::apply_operation(v, *this);
      }

      inline uint8_t * result_of_image( gil::any_image_t & m ) const
      {
        return result_of_view( boost::gil::view(m) );
      }
    };//struct get_image_pixels_f

    uint8_t * image::pixels()
    {
      if ( !_m /*|| !_v*/ ) return NULL;
      get_image_pixels_f fGetPixels;
      return _isView
        ? fGetPixels.result_of_view( _v->any() )
        : fGetPixels.result_of_image( _m->any() )
        ;
    }

    int image::width() const
    {
      if ( !_m /*|| !_v*/ ) return 0;
      return _isView ? _v->width() : _m->width();
    }

    int image::height() const
    {
      if ( !_m /*|| !_v*/ ) return 0;
      return _isView ? _v->height() : _m->height();
    }

    bool image::load( const std::string & file )
    {
      if ( !_m /*|| !_v*/ ) return false;

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

        if ( _isView ) { // convert into image
          delete _v;
          _v = NULL;
          _isView = 0;
          _m = new gil::image;
        }

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
