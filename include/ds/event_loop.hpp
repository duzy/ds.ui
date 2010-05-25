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

namespace ds
{
  struct event;

  /**
   *  @brief Event loop in a thread.
   */
  struct event_loop
  {
    void run();

    void post(const event &);
    void post_and_wait(const event &);

  protected:
    virtual on_event(const event &) = 0;

  private:
    bool wait_for_events();
    void get_next(event &);
  };//struct event_loop

}//namespace ds

#endif//__DS_EVENT_LOOP_HPP____by_Duzy_Chan__
