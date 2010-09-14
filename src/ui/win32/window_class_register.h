/**
 *    Copyright 2010-09-15 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

namespace ds { namespace ui { namespace detail {

      struct window_class_register
      {
        window_class_register( const std::wstring & className, WNDPROC proc );

        bool operator()();

        WNDCLASSEXW wc;
      };//struct window_class_register

    }//namespace detail
  }//namespace ui
}//namespace ds
