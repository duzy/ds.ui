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
    
    class event_pump
    {
    public:
      bool wait_for_events();
      void get_next(event &);
    };//class event_pump
    
  }//namespace ui
}//namespace ds

#endif//__DS_UI_EVENT_PUMP_HPP____by_Duzy_Chan__
