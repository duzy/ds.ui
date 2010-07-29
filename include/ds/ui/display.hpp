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
#       include <ds/shared_object.hpp>
#       include <ds/event_pump.hpp>

namespace ds { namespace ui {

    class screen;
    class window;

    /**
     *  @brief Windows is living in a particular screen of a display.
     */
    class display
      : boost::noncopyable
      , public shared_object<display>
      , public event_pump
    {
      display();
      ~display();

      struct IMPL;

      friend class screen;
      friend class window;

    public:
      class id
      {
        void * _p;

        friend class display;

      public:
        id(void * = 0);
      };

      // TODO:
      //        1) move semantic,
      //        2) think about 'close' display
      static pointer_t open( id i = id() );

      shared_object<screen>::pointer_t default_screen() const;
      shared_object<screen>::pointer_t get_screen( int index ) const;

      shared_object<window>::pointer_t default_root() const;

      void map( const shared_object<window>::pointer_t& win );
      void unmap( const shared_object<window>::pointer_t& win );
      bool has( const shared_object<window>::pointer_t& win );

    protected:
      virtual void pump_events(); //! must be thread safe
                                  //! must be the same thread as invoking 'open'
                                  //! must be the same thread as invoking 'start_pump'

    private:
      IMPL * _p;
    };//class display
    
  }//namespace ui
}//namespace ds

#endif//__DS_UI_DISPLAY_HPP____by_Duzy_Chan__
