/**                                                             -*- c++ -*-
 *
 */

#include <boost/test/unit_test.hpp>
#include <ds/shared_object.hpp>

struct simple_so : ds::shared_object<simple_so>
{
  static int instance_count;

  int value;

  explicit simple_so( int n ) : value(n) { ++instance_count; }
  ~simple_so() { --instance_count; }
};//struct simple_so

int simple_so::instance_count = 0;

static void as_arg_add_use_count( int originUseCount, simple_so::pointer p )
{
  BOOST_CHECK( p->use_count() == (originUseCount + 1) );
}

static void as_arg_no_add_use_count( int originUseCount, const simple_so::pointer & p )
{
  BOOST_CHECK( p->use_count() == originUseCount );
}

BOOST_AUTO_TEST_CASE( so_pointer )
{
  BOOST_CHECK( simple_so::instance_count == 0 );
  {
    simple_so::pointer p1( new simple_so(1) );

    BOOST_CHECK( p1->use_count() == 1 );
    BOOST_CHECK( simple_so::instance_count == 1 );

    as_arg_add_use_count( p1->use_count(), p1 );

    BOOST_CHECK( p1->use_count() == 1 );

    as_arg_no_add_use_count( p1->use_count(), p1 );

    BOOST_CHECK( p1->use_count() == 1 );

    simple_so::pointer p1_2( p1 );

    BOOST_CHECK( p1 == p1_2 );
    BOOST_CHECK( p1->use_count() == 2 );
    BOOST_CHECK( p1_2->use_count() == 2 );
    BOOST_CHECK( simple_so::instance_count == 1 );

    {
      simple_so::pointer p1_3( p1 );

      BOOST_CHECK( p1 == p1_3 );
      BOOST_CHECK( p1_2 == p1_3 );
      BOOST_CHECK( p1->use_count() == 3 );
      BOOST_CHECK( p1_2->use_count() == 3 );
      BOOST_CHECK( p1_3->use_count() == 3 );
      BOOST_CHECK( simple_so::instance_count == 1 );
    }

    {
      simple_so::pointer p2( new simple_so(2) );
      BOOST_CHECK( p1->use_count() == 1 );
      BOOST_CHECK( simple_so::instance_count == 2 );
    }
    BOOST_CHECK( simple_so::instance_count == 1 );
  }
  BOOST_CHECK( simple_so::instance_count == 0 );
}

BOOST_AUTO_TEST_CASE( so_weak_ref )
{
  simple_so::pointer p1( new simple_so(1) );
  
}

BOOST_AUTO_TEST_CASE( so_NO_CYCLE )
{
}
