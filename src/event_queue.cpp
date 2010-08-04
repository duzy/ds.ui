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
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

namespace ds {

  struct event_queue::IMPL
  {
    //typedef __gnu_cxx::new_allocator<event*> allocator_t;
    typedef __gnu_cxx::bitmap_allocator<event*> allocator_t;
    typedef std::deque<event*, allocator_t> queue_t;
    typedef boost::mutex mutex_t;
    typedef boost::condition condition_t;

    condition_t _condHasEvents;
    mutex_t _mutex;
    queue_t _events;

    IMPL()
      : _condHasEvents()
      , _mutex()
      , _events()
    {
      
    }
  };//struct event_queue::IMPL

  //////////////////////////////////////////////////////////////////////

  bool event_queue::has_events(uint32_t minType, uint32_t maxType) const
  {
    IMPL::mutex_t::scoped_lock l( _p->_mutex );
    IMPL::queue_t::const_iterator it( _p->_events.begin() );
    for(; it != _p->_events.end(); ++it) {
      if (minType <= (*it)->type && (*it)->type <= maxType) {
        return true;
      }
    }
    return false;
  }

  void event_queue::flush(uint32_t minType, uint32_t maxType)
  {
    IMPL::mutex_t::scoped_lock l( _p->_mutex );
    IMPL::queue_t::iterator it( _p->_events.begin() );
    for(; it != _p->_events.end();) {
      if (minType <= (*it)->type && (*it)->type <= maxType) {
        it = _p->_events.erase(it);
        continue;
      }
      ++it;
    }
  }

  event * event_queue::poll()
  {
    IMPL::mutex_t::scoped_lock l( _p->_mutex );
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
