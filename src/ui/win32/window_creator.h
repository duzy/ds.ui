/**
 *    Copyright 2010-09-15 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

namespace ds { namespace ui { namespace detail {

      /**
       *  @brief Helper class for creating Win32 window handlers.
       */
      struct window_creator
      {
        window_creator();

        HWND create( display::IMPL * disp ) const;

        DWORD exStyle;
        DWORD style;
        //std::wstring className;
        std::wstring windowName;
        int x;
        int y;
        int width;
        int height;
        HWND parent;
        HMENU menu;
        HINSTANCE instance;
        LPVOID param;
      };//struct window_creator
      
    }//namespace detail
  }//namespace ui
}//namespace ds
