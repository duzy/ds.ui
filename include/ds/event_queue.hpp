/**
 *    Copyright 2010-07-31 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#ifndef __DS_EVENT_QUEUE_HPP____by_Duzy_Chan__
#define __DS_EVENT_QUEUE_HPP____by_Duzy_Chan__ 1
#       include <boost/noncopyable.hpp>
#  if defined(HAS_STD_FUNCTION) && HAS_STD_FUNCTION
#       include <function>
#  else
#       include <boost/function.hpp>
#  endif

namespace ds
{
  struct event;

  /**
   *  @brief Provide functions for a thread-safe event queue.
   */
  class event_queue : boost::noncopyable // TODO: should be shared_object ??
  {
    struct IMPL;

  public:
    event_queue();
    ~event_queue();

    /**
     *  @brief Checks to see if certain event types are in the queue.
     */
    bool has_event(uint32_t type) const { return has_events(type, type); }
    bool has_events(uint32_t minType, uint32_t maxType) const;

    /**
     *  @brief Clears events of the specified type from the event queue.
     */
    void flush(uint32_t type) { return flush(type, type); }
    void flush(uint32_t minType, uint32_t maxType);

    /**
     *  @brief Polls for currently pending events.
     *
     *  Returns NULL if there are none pending events.
     */
    event * poll();

    /**
     *  @brief Waits indefinitely for the next available events.
     *
     *  Returns NULL if there was an error while waiting for events.
     */
    event * wait() { return wait(-1); }

    /**
     *  @brief Waits until the specified timeout(in milliseconds) for the next
     *         available event.
     *
     *  Returns NULL if there was an error while waiting for events.
     */
    event * wait(int timeout);

    enum PushResult // : int
      {
        PushOk,         // The event was successfully added to the queue
        PushFiltered,   // The event was filtered
        PushFull,       // The event queue was fulled
        PushError,      // Some error occured.
      };

    /**
     *  @brief Push an event to the queue.
     */
    PushResult push(event *);

    enum FilterResult // : bool
      {
        EventUnfiltered,
        EventFiltered,
      };

#   if defined(HAS_STD_FUNCTION) && HAS_STD_FUNCTION
    typedef std::function<FilterResult(event*)> filter_t;
#   else
    typedef boost::function<FilterResult(event*)> filter_t;
#   endif

    void set_filter(const filter_t & f);
    filter_t get_filter() const;

    /**
     *  @brief Run the filter function on the current event queue, removing
     *         any events for which the filter returns EventFilterd.
     */
    int filter_events();

    bool is_active() const;

    // TODO: event state (see SDL_events.h: SDL_EventState)
    // TODO: user-defined events (see SDL_events.h: SDL_RegisterEvents)

  private:
    IMPL *_p;
  };//class event_queue
}//namespace ds

#endif//__DS_EVENT_QUEUE_HPP____by_Duzy_Chan__
