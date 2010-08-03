/**
 *    Copyright 2010-08-02 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/event_queue.hpp>
#include <deque>
//#include <ext/new_allocator.h>
//#include <ext/pool_allocator.h>
#include <ext/bitmap_allocator.h>

namespace ds {

  struct event_queue::IMPL
  {
    //typedef __gnu_cxx::new_allocator<event*> allocator_t;
    typedef __gnu_cxx::bitmap_allocator<event*> allocator_t;
    
    std::deque<event*, allocator_t> events;
  };//struct event_queue::IMPL

  //////////////////////////////////////////////////////////////////////

  bool event_queue::has_events(uint32_t minType, uint32_t maxType)
  {
    return false;
  }

  void event_queue::flush(uint32_t minType, uint32_t maxType)
  {
  }

  event * event_queue::poll()
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
    return true;
  }

}//namespace ds
