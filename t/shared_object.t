/**                                                             -*- c++ -*-
 *
 */

#include <boost/test/unit_test.hpp>
#include <ds/shared_object.hpp>

struct simple_so : ds::shared_object<simple_so>
{
  static int instance_count;

  int value;

  weak_ref other;

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

    //as_arg_add_use_count( p1->use_count(), p1 );
    int n = p1->use_count();
    as_arg_add_use_count( n, p1 );

    BOOST_CHECK( p1->use_count() == 1 );

    //as_arg_no_add_use_count( p1->use_count(), p1 );
    as_arg_no_add_use_count( 1, p1 );

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
      BOOST_CHECK( p2->use_count() == 1 );
      BOOST_CHECK( simple_so::instance_count == 2 );
    }
    BOOST_CHECK( simple_so::instance_count == 1 );
  }
  BOOST_CHECK( simple_so::instance_count == 0 );
}

BOOST_AUTO_TEST_CASE( so_weak_ref )
{
  BOOST_CHECK( simple_so::instance_count == 0 );
  {
    simple_so::weak_ref r0;
    BOOST_CHECK( r0.use_count() == 0 );
    BOOST_CHECK( r0.weak_count() == 0 );
    BOOST_CHECK( r0.lock() == simple_so::pointer(NULL) );
  }

  simple_so::pointer p1( new simple_so(1) );
  simple_so::weak_ref r1( p1 );
  BOOST_CHECK( r1.use_count() == p1->use_count() );
  BOOST_CHECK( p1->use_count() == 1 );
  BOOST_CHECK( r1.weak_count() == 1 );
  BOOST_CHECK( simple_so::instance_count == 1 );

  {
    simple_so::pointer p1_1( r1.lock() );
    BOOST_CHECK( r1.use_count() == p1->use_count() );
    BOOST_CHECK( p1->use_count() == 2 );
    BOOST_CHECK( r1.weak_count() == 1 );
    BOOST_CHECK( simple_so::instance_count == 1 );
  }

  simple_so::weak_ref r2( p1 );
  BOOST_CHECK( r2.use_count() == p1->use_count() );
  BOOST_CHECK( p1->use_count() == 1 );
  BOOST_CHECK( r1.weak_count() == 2 );
  BOOST_CHECK( r2.weak_count() == 2 );
  BOOST_CHECK( simple_so::instance_count == 1 );

  simple_so::weak_ref r3( r2 );
  BOOST_CHECK( r3.use_count() == p1->use_count() );
  BOOST_CHECK( p1->use_count() == 1 );
  BOOST_CHECK( r1.weak_count() == 3 );
  BOOST_CHECK( r2.weak_count() == 3 );
  BOOST_CHECK( r3.weak_count() == 3 );
  BOOST_CHECK( simple_so::instance_count == 1 );
}

BOOST_AUTO_TEST_CASE( so_NO_CYCLE )
{
  BOOST_CHECK( simple_so::instance_count == 0 );
  {
    simple_so::pointer p1( new simple_so(1) );
    BOOST_CHECK( p1->use_count() == 1 );
    BOOST_CHECK( p1->other.weak_count() == 0 );
    BOOST_CHECK( simple_so::instance_count == 1 );

    p1->other = p1; // ref to itself

    BOOST_CHECK( simple_so::instance_count == 1 );
    BOOST_CHECK( p1->use_count() == 1 );
    BOOST_CHECK( p1->other.use_count() == 1 );
    BOOST_CHECK( p1->other.weak_count() == 1 );

    simple_so::pointer p2( new simple_so(2) );
    BOOST_CHECK( p2->use_count() == 1 );
    BOOST_CHECK( simple_so::instance_count == 2 );

    p2->other = p1->other; // another ref to p1

    BOOST_CHECK( simple_so::instance_count == 2 );
    BOOST_CHECK( p1->use_count() == 1 );
    BOOST_CHECK( p2->use_count() == 1 );
    BOOST_CHECK( p1->other.use_count() == 1 );
    BOOST_CHECK( p2->other.use_count() == 1 );
    BOOST_CHECK( p1->other.weak_count() == 2 );
    BOOST_CHECK( p2->other.weak_count() == 2 );

    simple_so::weak_ref r;
    r.swap( p2->other );

    BOOST_CHECK( p1->use_count() == 1 );
    BOOST_CHECK( p2->use_count() == 1 );
    BOOST_CHECK( p1->other.use_count() == 1 );
    BOOST_CHECK( p2->other.use_count() == 0 );
    BOOST_CHECK( p1->other.weak_count() == 2 );
    BOOST_CHECK( p2->other.weak_count() == 0 );
    BOOST_CHECK( r.use_count() == 1 );
    BOOST_CHECK( r.weak_count() == 2 );

    r.reset();

    BOOST_CHECK( p1->use_count() == 1 );
    BOOST_CHECK( p1->other.use_count() == 1 );
    BOOST_CHECK( p1->other.weak_count() == 1 );
    BOOST_CHECK( r.use_count() == 0 );
    BOOST_CHECK( r.weak_count() == 0 );
  }
  BOOST_CHECK( simple_so::instance_count == 0 );
}
