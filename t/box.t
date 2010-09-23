/**                                                             -*- c++ -*-
 *
 */

#include <boost/test/unit_test.hpp>
#include <ds/graphics/region.hpp>
#include <boost/geometry/algorithms/make.hpp>

using namespace ds::graphics;
using boost::geometry::make;

BOOST_AUTO_TEST_CASE( box_assign )
{
  box b = make<box>( 10, 10, 110, 110 );
  BOOST_CHECK( b.left()   == 10 );
  BOOST_CHECK( b.top()    == 10 );
  BOOST_CHECK( b.right()  == 110 );
  BOOST_CHECK( b.bottom() == 110 );
  BOOST_CHECK( b.x()      == 10 );
  BOOST_CHECK( b.y()      == 10 );
  BOOST_CHECK( b.width()  == 100 );
  BOOST_CHECK( b.height() == 100 );
  b.left      ( 20 );
  b.top       ( 20 );
  b.right     ( 120 );
  b.bottom    ( 120 );
  BOOST_CHECK( b.left()   == 20 );
  BOOST_CHECK( b.top()    == 20 );
  BOOST_CHECK( b.right()  == 120 );
  BOOST_CHECK( b.bottom() == 120 );
  BOOST_CHECK( b.x()      == 20 );
  BOOST_CHECK( b.y()      == 20 );
  BOOST_CHECK( b.width()  == 100 );
  BOOST_CHECK( b.height() == 100 );

  b.x( 25 ), b.y( 25 );
  BOOST_CHECK( b.x()      == 25 );
  BOOST_CHECK( b.y()      == 25 );
  BOOST_CHECK( b.width()  == 95 );
  BOOST_CHECK( b.height() == 95 );
}
