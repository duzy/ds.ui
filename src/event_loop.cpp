/**
 *    Copyright 2010-08-02 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/event_loop.hpp>
#include <ds/event_queue.hpp>
#include <ds/event.hpp>

namespace ds {

  int event_loop::run()
  {
    // TODO: event loop implentation... handles QUIT event
    event * evt = NULL;
    while (evt = _queue->wait()) {
      if (evt->type == event::quit::TypeValue) {
        return 0;
      }
    }
    return -1;
  }

  void event_loop::on_event(const event &)
  {
    // Does nothing.
  }
  
}//namespace ds
