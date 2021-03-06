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
#include <iostream>

struct my_window : ds::ui::window
{
  explicit my_window( const ds::ui::display::pointer & disp )
    : ds::ui::window( disp )
  {
  }

protected:
  void on_render( ds::graphics::canvas & canvas )
  {
    dsL("simple render");
    using namespace boost::geometry;
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
      <point_2d, ds::graphics::point> map(4,-1,10,7,800,600);
    transform(poly, g, map);

    canvas.render( ds::graphics::color::argb(1.0, 0.99, 0.1, 0.0) );
    canvas.render( g );
    canvas.stroke( g );
  }
};//struct my_window


int main(int argc, char** argv)
{
  for (int n=0; n < argc; ++n) {
    std::clog<<"arg["<<n<<"]: "<<argv[n]<<std::endl;
  }

  // make a default display connection
  ds::ui::display::pointer disp = ds::ui::display::open();
  my_window::pointer win( new my_window(disp) );

  //dsI( !win->is_visible() );
  dsI( disp->has(win) );

  win->show();

  int n;
  {
    ds::ui::event_loop loop( disp );
    n = loop.run();
  }

  return n;
}
