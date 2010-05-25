/**
 *    Copyright 2010-07-18 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/ui/display.hpp>
#include <ds/ui/window.hpp>
#include <cassert>

int main(int argc, char** argv)
{
  ds::ui::display disp; // make a default display connection
  ds::ui::window win(disp); // a window in the display 'disp'
  ds::ui::window win2; // a window of no display is trivial

  assert( disp.has(win) );
  
  win.show();

  disp.add( win2 );
  assert( disp.has(win2) );

  win2.show(); // show it since it's belong to 'disp'
  
  return disp.reduce_events();
}
