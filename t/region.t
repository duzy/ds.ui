/**                                                             -*- c++ -*-
 *
 */

#include <boost/test/unit_test.hpp>
#include <ds/graphics/region.hpp>
#include <boost/geometry/algorithms/make.hpp>

using namespace ds::graphics;
using boost::geometry::make;

BOOST_AUTO_TEST_CASE( region_merge )
{
  box b1(make<box>( 10, 10, 110, 110 ));
  region reg1( b1 );
}
