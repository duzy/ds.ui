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
#include <ds/graphics/segment.hpp>
#include <ds/graphics/ring.hpp>
#include <ds/graphics/region.hpp>
#include <ds/graphics/drawing_tools.hpp>
#include <ds/graphics/image.hpp>
#include <ds/resource/streambuf.hpp>
#include <boost/geometry/algorithms/make.hpp>
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
  my_window()
  {
    ds::resource::streambuf buf("/eyes.png");
    std::istream is( &buf );
    bool ok = image.load( is );
    dsEif( !ok, "cannot load '/eyes.png'" );
    if (ok) {
      dsL("test-image-size: "<<image.width()<<"x"<<image.height());
    }
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

    canvas.render( ds::graphics::color::rgba(0.9, 0.1, 0.1, 1.0) );

    ds::graphics::brush brush;
    ds::graphics::pen pen;
    {
      ds::graphics::polygon g;
      strategy::transform::map_transformer
        <point_2d, ds::graphics::point> map(4,-1,10,7,800,600);
      transform(poly, g, map);
      pen.color = ds::graphics::color::rgb(0.3, 0.3, 0.6);
      brush.color = ds::graphics::color::rgba(0.6, 0.3, 0.3, 0.5);
      canvas.render( g, brush );
      canvas.stroke( g, pen );
    }
    {
      pen.color = ds::graphics::color::rgb(0.8, 0.1, 0.1);
      canvas.stroke( make<ds::graphics::box>( 100, 50, 150, 150 ), pen );
      pen.color = ds::graphics::color::rgb(0.1, 0.8, 0.1);
      canvas.stroke( make<ds::graphics::box>( 105, 55, 155, 155 ), pen );
      pen.color = ds::graphics::color::rgb(0.1, 0.1, 0.8);
      canvas.stroke( make<ds::graphics::box>( 110, 60, 160, 160 ), pen );
    }
    {
      ds::graphics::point p1({32, 74}), p2({460, 227});
      ds::graphics::segment s(p1, p2);
      pen.color = ds::graphics::color::rgb(0.5, 0.8, 0.5);
      canvas.stroke( s, pen );
    }
    {
      ds::graphics::ring r;
      {
        const double coords[][2] = {
          {40, 25}, {170, 34}, {260, 87}, {190, 230}, {52, 75},
          {40, 25} // closing point is opening point
        };
        assign(r, coords);
      }
      pen.color = ds::graphics::color::rgb(0.1, 0.8, 0.1);
      canvas.stroke( r, pen );
    }
    {
      //image.convert_pixels( image.ABGR_8888_PIXEL );
      //image.convert_pixels( image.RGBA_8888_PIXEL );
      image.convert_pixels( ds::graphics::image::PixelType(canvas.sys_pixel_type()) );
      canvas.render( image, 100, 200 );
    }
    {
      ds::graphics::polygon g;
      {
        const ds::graphics::coordinate_t coords[][2] = {
          {10.0, 10.0}, {110.0, 20.0}, {110.0, 210.0}, {10.0, 210.0},
          {10.0, 10.0} // closing point is opening point
        };
        assign(g, coords);
      }
      {
        g.inners().resize(1);
        const double coords[][2] = {
          {40, 20}, {70, 14}, {48, 90}, {22, 80},
          {40, 20} // closing point is opening point
        };
        assign(g.inners().back(), coords);
      }

      canvas.clip( g );
      canvas.render( ds::graphics::color::rgba(0.1, 0.1, 0.5, 0.5) );
      //canvas.restore();
    }
  }//on_render

private:
  ds::graphics::image image;
};

int main(int argc, char** argv)
{
  for (int n=0; n < argc; ++n) {
    std::clog<<"arg["<<n<<"]: "<<argv[n]<<std::endl;
  }

  // make a default display connection
  ds::ui::display::pointer disp = ds::ui::display::open();
  //dsL("display-refs: "<<disp->use_count());

  ds::ui::screen::pointer scrn = disp->default_screen();
  //dsL("screen-refs: "<<scrn->use_count());

  //ds::ui::window win1( disp ); // a window in the display 'disp'
  ds::ui::window::pointer win1( new ds::ui::window(disp) );
  assert( disp->has(win1) );

  win1->show();

  my_window::pointer win2( new my_window );
  disp->map( win2 );
  assert( disp->has(win2) );

  win2->show(); // show it since it's belong to 'disp'

  //dsL("display-refs: "<<disp->use_count());

  int n;
  {
    ds::ui::event_loop loop( disp );
    //dsL("display-refs: "<<disp->use_count());
    n = loop.run();
  }

  //dsL("display-refs: "<<disp->use_count());
  return n;
}
