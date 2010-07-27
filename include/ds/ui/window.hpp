/**
 *    Copyright 2010-07-18 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/
#ifndef __DS_UI_WINDOW_HPP____by_Duzy_Chan__
#define __DS_UI_WINDOW_HPP____by_Duzy_Chan__ 1
#       include <ds/shared_object.hpp>

namespace ds { namespace ui {

    class display;

    class window : public shared_object<window>
    {
      struct IMPL;

      friend class display;

    public:
      /**
       *  @brief Construct a trivial window.
       */
      window();

      /**
       *  @brief Construct a window in the display 'disp' and be ready for
       *         showing.
       */
      window( display * disp );

      display * get_display() const;

      void show();
      void hide();
      void close();

      void move( int x, int y );
      void resize( int w, int h );

    private:
      IMPL * _p;
    };//class window

  }//namespace ui
}//namespace ds

#endif//__DS_UI_WINDOW_HPP____by_Duzy_Chan__
