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
      display();
      ~display();

      struct IMPL;

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
      static display * open( id i = id() );

      int width() const;
      int height() const;

      window *root() const;

      void add( window *win );
      void remove( window *win );
      bool has( window *win );

      unsigned black_pixel() const;
      unsigned white_pixel() const;

      int reduce_events( window * win );

    private:
      IMPL * _p;
    };//class display
    
  }//namespace ui
}//namespace ds

#endif//__DS_UI_DISPLAY_HPP____by_Duzy_Chan__
