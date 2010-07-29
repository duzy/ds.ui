/**
 *    Copyright 2010-08-02 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/event_loop.hpp>
#include <ds/event_queue.hpp>
#include <ds/event.hpp>

namespace ds {

  int event_loop::run()
  {
    event * evt = NULL;
    while ( _queue->is_active() ) {
      /** This enables UI event loop to pump events in-loop.
       *  SDL does pumping events before wait(SDL_WaitEventTimeOut)
       */
      this->loop_in();

      if (!(evt = _queue->wait(10))) {
        // TODO: no event or got error?
        continue;
      }

      bool quit( event::quit::is(evt) );

      this->on_event(*evt);

      delete evt; // TODO: avoid using 'event *' and this delete command
      //_queue->revoke_event(evt);
      evt = NULL;

      if ( quit ) return 0; // TODO: deactive the event queue
    }
    return -1;
  }

  void event_loop::loop_in()
  {
    // Does nothing by default, the derived should do pumping on it's own
  }

  void event_loop::on_event(const event &)
  {
    // Does nothing, just allow the derived class to invoke
    // 'ds::event_loop::on_event' in it's own 'on_event' implenmentation.
  }
  
}//namespace ds
