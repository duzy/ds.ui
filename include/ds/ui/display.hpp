/**
 *    Copyright 2010-05-25 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/
#ifndef __DS_UI_DISPLAY_HPP____by_Duzy_Chan__
#define __DS_UI_DISPLAY_HPP____by_Duzy_Chan__ 1
#       include <boost/noncopyable.hpp>

namespace ds { namespace ui {

    class window;

    /**
     *  @brief Windows is living in a particular display.
     */
    class display : boost::noncopyable
    {
    public:
      void add(window &);
      void remove(window &);
      bool has(window &);
      int reduce_events();
    };//class display
    
  }//namespace ui
}//namespace ds

#endif//__DS_UI_DISPLAY_HPP____by_Duzy_Chan__

