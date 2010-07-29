/**
 *    Copyright 2010-05-25 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/
#ifndef __DS_UI_EVENT_PUMP_HPP____by_Duzy_Chan__
#define __DS_UI_EVENT_PUMP_HPP____by_Duzy_Chan__ 1

namespace ds { namespace ui {

    struct event;
    class event_queue;

    /**
     *  @brief Pumps platform events into the specified event queue. 
     */
    class event_pump
    {
      event_pump(event_queue * q) : _queue(q) {}
      
      virtual ~event_pump() {}

      // TODO: shold start a new thread to pump events
      void start_pump()
      {
        while (_queue.is_active()) { // QUIT message should deactivate the queue
          this->pump_events();
        }
      }

    protected:
      /**
       *  This do the real task of pumping events and should be thread safety.
       */
      virtual void pump_events() = 0;

    private:
      event_queue *_queue;
    };//class event_pump
    
  }//namespace ui
}//namespace ds

#endif//__DS_UI_EVENT_PUMP_HPP____by_Duzy_Chan__
