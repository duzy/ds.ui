/**
 *    Copyright 2010-07-18 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/ui/display.hpp>
#include <ds/ui/screen.hpp>
#include <ds/ui/window.hpp>
#include <ds/ui/events.hpp>
#include <ds/graphics/canvas.hpp>
#include <ds/graphics/color.hpp>
#include <ds/graphics/polygon.hpp>
#include <boost/geometry/algorithms/assign.hpp>
#include <boost/geometry/algorithms/transform.hpp>
#include <boost/geometry/strategies/transform/map_transformer.hpp>
#include <boost/geometry/geometries/cartesian2d.hpp>
#include <boost/geometry/geometries/adapted/c_array_cartesian.hpp>
#include <ds/debug.hpp>
#include <cassert>

namespace cycle
{
  struct so_1;
  struct so_2;
  struct so_3;
  struct so_4;

  static int object_count_1 = 0;
  static int object_count_2 = 0;
  static int object_count_3 = 0;
  static int object_count_4 = 0;

  struct so_1 : ds::shared_object<so_1>
  {
    ds::shared_object<so_2>::pointer _2;

    so_1() { ++object_count_1; }
    ~so_1() { --object_count_1; }
  };

  struct so_2 : ds::shared_object<so_2>
  {
    ds::shared_object<so_1>::pointer _1;
    ds::shared_object<so_3>::pointer _3;

    so_2() { ++object_count_2; }
    ~so_2() { --object_count_2; }
  };

  struct so_3 : ds::shared_object<so_3>
  {
    ds::shared_object<so_1>::pointer _1;
    ds::shared_object<so_4>::pointer _4;

    so_3() { ++object_count_3; }
    ~so_3() { --object_count_3; }
  };

  struct so_4 : ds::shared_object<so_4>
  {
    ds::shared_object<so_1>::pointer _1;

    so_4() { ++object_count_4; }
    ~so_4() { --object_count_4; }
  };
}

struct so_test : ds::shared_object<so_test> {};

static void test_so_f( so_test::pointer p, int n )
{
  dsI( 0 < p->refcount() );
  dsI( p->refcount() == n + 1 );
}

struct my_window : ds::ui::window
{
protected:
  void on_render( ds::graphics::canvas & canvas )
  {
    using namespace boost::geometry;
    ds::graphics::paint paint;
    polygon_2d poly;
    {
      const double coords[][2] = {
        {2.0, 1.3}, {2.4, 1.7}, {2.8, 1.8}, {3.4, 1.2}, {3.7, 1.6},
        {3.4, 2.0}, {4.1, 3.0}, {5.3, 2.6}, {5.4, 1.2}, {4.9, 0.8}, {2.9, 0.7},
        {2.0, 1.3} // closing point is opening point
      };
      assign(poly, coords);
    }
    {
      poly.inners().resize(1);
      linear_ring<point_2d> & inner = poly.inners().back();

      const double coor[][2] = { {4.0, 2.0}, {4.2, 1.4}, {4.8, 1.9}, {4.4, 2.2}, {4.0, 2.0} };
      assign(inner, coor);
    }

    ds::graphics::polygon g;
    strategy::transform::map_transformer
      <point_2d, ds::graphics::point> map(4,-1,10,7,400,300);
    transform(poly, g, map);

    canvas.render( ds::graphics::color(1.0, 0.9, 0.1, 0.1) );
    canvas.render( g, paint );
    canvas.stroke( g, paint );
  }
};

static void test_shared_object()
{
  so_test *p( new so_test );
  dsI( p->refcount() == 0 );

  so_test::pointer sp( p );
  dsI( p->refcount() == 1 );
  dsI( p->refcount() == sp->refcount() );

  so_test::pointer sp2( p );
  dsI( p->refcount() == 2 );
  dsI( p->refcount() == sp2->refcount() );
  dsI( sp->refcount() == sp2->refcount() );

  {
    so_test::pointer sp3( p );
    dsI( p->refcount() == 3 );
  }
  dsI( p->refcount() == 2 );

  {
    so_test::pointer sp4( sp );
    dsI( p->refcount() == 3 );
  }
  dsI( p->refcount() == 2 );
  
  test_so_f( p, 2 );
  test_so_f( p, p->refcount() );

  /*
  so_test so;
  dsI( so.refcount() == 0 );
  {
    so_test::pointer p( &so );
    dsI( so.refcount() == 1 );
    dsI( p->refcount() == 1 );
  }
  */

  {
    cycle::so_1::pointer _1( new cycle::so_1 );
    cycle::so_2::pointer _2( new cycle::so_2 );
    cycle::so_3::pointer _3( new cycle::so_3 );
    cycle::so_4::pointer _4( new cycle::so_4 );
    
    /* Cycle Leak 1 */
    //_1->_2 = _2;
    //_2->_1 = _1;

    /* Cycle Leak 2 */
    //_1->_2 = _2;
    //_2->_3 = _3;
    //_3->_1 = _1;

    /* Cycle Leak 3 */
    _1->_2 = _2;
    _2->_3 = _3;
    _3->_4 = _4;
    //_4->_1 = _1;
  }
  dsI( cycle::object_count_1 == 0 );
  dsI( cycle::object_count_2 == 0 );
  dsI( cycle::object_count_3 == 0 );
  dsI( cycle::object_count_4 == 0 );
}

int main(int argc, char** argv)
{
  test_shared_object();

  // make a default display connection
  ds::ui::display::pointer disp = ds::ui::display::open();
  dsL("display-refs: "<<disp->refcount());

  ds::ui::screen::pointer scrn = disp->default_screen();
  dsL("screen-refs: "<<scrn->refcount());

  //ds::ui::window win1( disp ); // a window in the display 'disp'
  ds::ui::window::pointer win1( new ds::ui::window(disp) );
  assert( disp->has(win1) );

  win1->show();

  //ds::ui::window win2; // a window of no display is trivial
  my_window::pointer win2( new my_window );
  disp->map( win2 );
  assert( disp->has(win2) );

  win2->show(); // show it since it's belong to 'disp'

  dsL("display-refs: "<<disp->refcount());

  int n;
  {
    ds::ui::event_loop loop( disp );
    dsL("display-refs: "<<disp->refcount());
    n = loop.run();
  }

  dsL("display-refs: "<<disp->refcount());
  return n;
}
