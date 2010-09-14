/**
 *    Copyright 2010-09-15 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <windows.h>
#include <string>
#include "window_class_register.h"

namespace ds { namespace ui { namespace detail {

      window_class_register::window_class_register( const std::wstring & className, WNDPROC proc )
      {
        wc.cbSize             = sizeof(WNDCLASSEXW);
        wc.style              = 0;
        wc.lpfnWndProc        = proc;
        wc.cbClsExtra         = 0;
        wc.cbWndExtra         = 0;
        wc.hInstance          = 0; //::GetModuleInstance();
        wc.hIcon              = LoadIcon( NULL, IDI_APPLICATION );
        wc.hIconSm            = LoadIcon( NULL, IDI_APPLICATION );
        wc.hCursor            = LoadCursor( NULL, IDC_ARROW );
        wc.hbrBackground      = (HBRUSH)(COLOR_BTNSHADOW);
        wc.hbrBackground      = (HBRUSH)(COLOR_WINDOWFRAME);
        wc.lpszMenuName       = NULL;
        wc.lpszClassName      = className.c_str();
      }

      bool window_class_register::operator()()
      {
        //  Documentation info from MSDN:
        //
        //If the function succeeds, the return value is a class atom that
        //uniquely identifies the class being registered. This atom can only
        //be used by the CreateWindow, CreateWindowEx, GetClassInfo,
        //GetClassInfoEx, FindWindow, FindWindowEx, and UnregisterClass
        //functions and the IActiveIMMap::FilterClientWindows method.
        //
        //If the function fails, the return value is zero. To get extended
        //error information, call GetLastError. 

        return ( ::RegisterClassExW( &wc ) != 0 );
      }

    }//namespace detail
  }//namespace ui
}//namespace ds
