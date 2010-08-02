/**
 *    Copyright 2010-08-02 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/event_queue.hpp>

namespace ds {

  bool event_queue::has_event(uint32_t type)
  {
    return false;
  }

  bool event_queue::has_events(uint32_t minType, uint32_t maxType)
  {
    return false;
  }

  void event_queue::flush(uint32_t type)
  {
  }

  void event_queue::flush(uint32_t minType, uint32_t maxType)
  {
  }

  event * event_queue::poll()
  {
    return 0;
  }

  event * event_queue::wait()
  {
    return 0;
  }

  event * event_queue::wait(int timeout)
  {
    return 0;
  }

  event_queue::PushResult event_queue::push(event *)
  {
    return PushError;
  }

  void event_queue::set_filter(const filter_t & f)
  {
  }

  event_queue::filter_t event_queue::get_filter() const
  {
    return event_queue::filter_t(0);
  }

  int event_queue::filter_events()
  {
    return 0;
  }

  bool event_queue::is_active() const
  {
    return false;
  }

}//namespace ds
