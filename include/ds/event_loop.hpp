/**
 *    Copyright 2010-07-18 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#ifndef __DS_EVENT_LOOP_HPP____by_Duzy_Chan__
#define __DS_EVENT_LOOP_HPP____by_Duzy_Chan__ 1
#       include <boost/noncopyable.hpp>

namespace ds
{
  struct event;
  class event_queue;

  /**
   *  @brief Event loop in a thread.
   *
   *  @usage
   *  @code
   *  
   *    TODO: check the usage of boost::member_from_base
   *    
   *    class MyLoop
   *      : boost::member_from_base<ds::event_queue>
   *      , ds::event_loop
   *    {
   *    public:
   *      MyLoop()
   *        : ds::event_loop( &boost::member_from_base<ds::event_queue>::member )
   *        , _pump( &boost::member_from_base<ds::event_queue>::member )
   *        , _pump2( &boost::member_from_base<ds::event_queue>::member )
   *      {
   *        _pump.start_pump(); // event pump and MyLoop should be running in
   *                            // different threads
   *        // or: _pump.start_pump_in_new_thread();
   *
   *        _pump2.start_pump();
   *      }
   *      
   *    protected:
   *      virtual on_event(const event &)
   *      {
   *            // dispatch events here...
   *      }
   *
   *    private:
   *      MyEventPump _pump;
   *      AnotherEventPump _pump2;
   *    };
   *
   *    void run_loop()
   *    {
   *      MyLoop loop;
   *      loop.run(); // run the event loop in the current thread
   *    }
   *  @endcode
   */
  class event_loop : boost::noncopyable
  {
  public:
    event_loop(event_queue * q) : _queue(q) {}

    virtual ~event_loop() {}

    int run();

  protected:
    virtual void on_event(const event &) = 0;

  private:
    event_queue *_queue;
  };//class event_loop

}//namespace ds

#endif//__DS_EVENT_LOOP_HPP____by_Duzy_Chan__
