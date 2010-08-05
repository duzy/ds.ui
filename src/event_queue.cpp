/**
 *    Copyright 2010-08-02 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/event.hpp>
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

    int _isActive : 1; // QUIT event will unset this flag

    IMPL()
      : _condHasEvents()
      , _mutex()
      , _events()
      , _isActive(1)
    {
      
    }
  };//struct event_queue::IMPL

  //////////////////////////////////////////////////////////////////////

  event_queue::event_queue()
    : _p( new IMPL )
  {
  }

  event_queue::~event_queue()
  {
    delete _p;
  }

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
    if ( _p->_events.empty() ) return 0;
    event * evt( _p->_events.front() );
    _p->_events.pop_front();
    return evt;
  }

  event * event_queue::wait(int timeout)
  {
    IMPL::mutex_t::scoped_lock l( _p->_mutex );
    while ( _p->_events.empty() ) _p->_condHasEvents.wait(l);
    event * evt( _p->_events.front() );
    _p->_events.pop_front();
    return evt;
  }

  event_queue::PushResult event_queue::push(event *evt)
  {
    IMPL::mutex_t::scoped_lock l( _p->_mutex );
    // TODO: handle with PushFiltered, PushFull, PushError
    _p->_events.push_back( evt );
    _p->_condHasEvents.notify_one();
    return PushOk;
  }

  void event_queue::set_filter(const filter_t & f)
  {
    // TODO: ...
  }

  event_queue::filter_t event_queue::get_filter() const
  {
    // TODO: ...
    return event_queue::filter_t(0);
  }

  int event_queue::filter_events()
  {
    // TODO: ...
    return 0;
  }

  bool event_queue::is_active() const
  {
    return bool( _p->_isActive );
  }

}//namespace ds
