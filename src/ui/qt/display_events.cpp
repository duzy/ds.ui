/**
 *    Copyright 2010-08-06 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
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
#include <ds/debug.hpp>
#include <boost/geometry/algorithms/make.hpp>
#include "../window_impl.h"
#include "../screen_impl.h"
#include "../display_impl.h"
#include <ds/event_queue.hpp>

namespace ds { namespace ui {

    void display::IMPL::push_event( event_queue *eq, QEvent * event )
    {
      
    }

  }//namespace ui
}//namespace ds
