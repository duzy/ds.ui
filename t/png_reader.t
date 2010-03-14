/**      							-*- c++ -*-
 *
 */

#include <ds/graphics/detail/png_reader.hpp>
#include <ds/graphics/detail/image.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>
#include <fstream>

BOOST_AUTO_TEST_CASE( png_reader )
{
  std::ifstream ifs( "eyes.png", ifs.binary|ifs.in );
  if ( !ifs ) ifs.open( "t/eyes.png", ifs.binary|ifs.in );
  BOOST_CHECK( ifs );

  ds::graphics::detail::png_reader rdr( ifs );
  ds::graphics::detail::image::any_t img;
  rdr.read_image( img );
  BOOST_CHECK( img.width() == 60 );
  BOOST_CHECK( img.height() == 60 );

  boost::gil::png_write_view( "test-out.png", view(img) );

  //== Check test-out.png
  std::ifstream ifs2( "test-out.png", ifs2.binary|ifs2.in );
  BOOST_CHECK( ifs2 );
  ds::graphics::detail::image::any_t img2;
  rdr.read_image( img2 );
  BOOST_CHECK( img2.width() == 60 );
  BOOST_CHECK( img2.height() == 60 );
}
