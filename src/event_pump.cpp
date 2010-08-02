/**
 *    Copyright 2010-08-02 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/event_pump.hpp>
#include <ds/event_queue.hpp>

namespace ds {

  void event_pump::start_pump()
  {
    // TODO: shold start a new thread to pump events or just making
    //       this method thread safe?
    while (_queue->is_active()) { // TODO: QUIT message should deactivate the queue
      this->pump_events();
    }
  }

}//namespace ds
