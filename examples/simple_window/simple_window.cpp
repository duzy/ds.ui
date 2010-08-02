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
#include <cassert>
#include <iostream>

int main(int argc, char** argv)
{
  // make a default display connection
  ds::ui::display::pointer_t disp = ds::ui::display::open();
  ds::ui::screen::pointer_t scrn = disp->default_screen();

  ds::ui::window win1( disp ); // a window in the display 'disp'
  assert( disp->has(&win1) );

  win1.show();

  ds::ui::window win2; // a window of no display is trivial
  disp->map( &win2 );
  assert( disp->has(&win2) );

  win2.show(); // show it since it's belong to 'disp'
  
  std::cout << "loop..." <<std::endl;
  //int n = disp->reduce_events();
  ds::ui::event_loop loop( disp );
  int n = loop.run();

  std::cout << n <<std::endl;
  return n;
}
