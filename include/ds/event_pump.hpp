/**
 *    Copyright 2010-05-25 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/
#ifndef __DS_EVENT_PUMP_HPP____by_Duzy_Chan__
#define __DS_EVENT_PUMP_HPP____by_Duzy_Chan__ 1

namespace ds { 

  struct event;
  class event_queue;

  /**
   *  @brief Pumps platform events into the specified event queue. 
   */
  class event_pump
  {
  public:
    virtual ~event_pump() {}

    void start_pump();

  protected:
    event_pump(event_queue * q) : _queue(q) {}

    /**
     *  This do the real task of pumping events and should be thread safety.
     */
    virtual void pump_events() = 0;

    event_queue *get_queue() const { return _queue; }

  private:
    event_queue *_queue;
  };//class event_pump
    
}//namespace ds

#endif//__DS_EVENT_PUMP_HPP____by_Duzy_Chan__
