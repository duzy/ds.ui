#include <ds/resource/item.hpp>
#include <ds/resource/item.ipp>
#include <ds/resource/streambuf.hpp>
#include <ds/graphics/detail/png_reader.hpp>
#include <ds/graphics/detail/image.hpp>
#include <ds/debug.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>
#include <iostream>

int main( int argc, char** argv )
{
  std::clog<<"----"<<std::endl;
  {
    ds::resource::item a( "/images/a.png" );
    ds::resource::item b( "/images/b.png" );
    std::clog<<a.name()<<": size="<<a.size()<<std::endl;
    std::clog<<b.name()<<": size="<<b.size()<<std::endl;

    ds::resource::streambuf bufA( "/images/a.png" );
    ds::resource::streambuf bufB( "/images/b.png" );
    assert( bufA.is_valid() );
    assert( bufB.is_valid() );

    {
      std::istream is( &bufA );
      ds::graphics::detail::png_reader r( is );
      ds::graphics::detail::image::any_t img;
      r.read_image( img );
      assert( img.width() == 60 );
      assert( img.height() == 60 );
      boost::gil::png_write_view( "test-out-a.png", view(img) );
    }
    {
      std::istream is( &bufB );
      ds::graphics::detail::png_reader r( is );
      ds::graphics::detail::image::any_t img;
      r.read_image( img );
      assert( img.width() == 32 );
      assert( img.height() == 32 );
      boost::gil::png_write_view( "test-out-b.png", view(img) );
    }
  }
  {
    ds::resource::item a( "/skins/image-a.png" );
    ds::resource::item b( "/skins/image-b.png" );
    std::clog<<a.name()<<": size="<<a.size()<<std::endl;
    std::clog<<b.name()<<": size="<<b.size()<<std::endl;

    ds::resource::streambuf bufA( "/skins/image-a.png" );
    ds::resource::streambuf bufB( "/skins/image-b.png" );
    assert( bufA.is_valid() );
    assert( bufB.is_valid() );

    {
      std::istream is( &bufA );
      ds::graphics::detail::png_reader r( is );
      ds::graphics::detail::image::any_t img;
      r.read_image( img );
      assert( img.width() == 60 );
      assert( img.height() == 60 );
      boost::gil::png_write_view( "test-out-skin-a.png", view(img) );
    }
    {
      std::istream is( &bufB );
      ds::graphics::detail::png_reader r( is );
      ds::graphics::detail::image::any_t img;
      r.read_image( img );
      assert( img.width() == 32 );
      assert( img.height() == 32 );
      boost::gil::png_write_view( "test-out-skin-b.png", view(img) );
    }
  }
  return 0;
}
