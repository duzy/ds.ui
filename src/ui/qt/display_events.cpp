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

    bool display::IMPL::push_event( event_queue *eq, QObject * receiver, QEvent * event )
    {
      if ( !receiver->isWidgetType() )
        return false;

      window_map_t::iterator wit = _winmap.find( qobject_cast<QWidget*>(receiver) );
      if (wit == _winmap.end())
        return false;

      window::pointer win( wit->second );                         dsI( win );

      switch ( event->type() ) {
      case QEvent::Close:
        dsL("close");
        // TODO: should notify the window of the close event
        break;

      case QEvent::Destroy:
        dsL("destroy");
        erase_destroyed_window( eq, wit->first, win );
        dsI( win->_p->_native_win == NULL );
        break;
      }
      return false;
    }

  }//namespace ui
}//namespace ds
