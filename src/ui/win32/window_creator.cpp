/**
 *    Copyright 2010-09-15 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#define WIN32_LEAN_AND_MEAN
#include <ds/ui/window.hpp>
#include <ds/ui/screen.hpp>
#include <ds/ui/display.hpp>
#include "../window_impl.h"
#include "../display_impl.h"
#include <commctrl.h>
#include <ds/debug.hpp>
#include "window_class_register.h"
#include "window_creator.h"
#include "error_message.h"

namespace ds { namespace ui { namespace detail {

      /**
       *  @brief Win32 HWND creator constructor
       */
      window_creator::window_creator()
        : exStyle( WS_EX_APPWINDOW )
        , style( WS_OVERLAPPEDWINDOW )
          //, className( s_strWindowClassName )
        , windowName( L"ds::ui::window.noname" )
        , x( CW_USEDEFAULT )
        , y( CW_USEDEFAULT )
        , width( CW_USEDEFAULT )
        , height( CW_USEDEFAULT )
        , parent( NULL )
        , menu( NULL )
        , instance( NULL )
        , param( NULL )
      {
        /*
        if(dialog) {
          if(parent && modal) {
            dwExStyle = 0;	// Modal Popup-Window
            dwStyle = dwStyle | DS_MODALFRAME;
          }
          else if(parent)
            dwExStyle = 0;
          dwStyle = dwStyle | WS_POPUPWINDOW | WS_CAPTION ;	// normal Popup-Window
        }
        else
          dwStyle = WS_OVERLAPPEDWINDOW | WS_TABSTOP;	// Application Window
        */
      }

      /**
       *  @brief Creates an hwnd. 
       */
      HWND window_creator::create( display::IMPL * disp ) const
      {
        //!< Initialize Win32 common controls
        ::InitCommonControls();

        const wchar_t * wc = disp->get_window_class_name( true );  dsI( wc );

        HWND hWnd =
          ::CreateWindowExW( exStyle,
                             wc, //className.c_str(),
                             windowName.c_str(),
                             style,
                             x, y, width, height,
                             parent,
                             menu,
                             instance,
                             param
                             );

        if ( hWnd == NULL ) {
          std::wstring lem = detail::get_last_error_message();
          std::wstring errorMessage( L"Failed to create window!" );
          errorMessage += L"\n  ";
          errorMessage += lem;
          std::wcerr<<"error: "<<lem<<std::endl;
          detail::error_message_box( errorMessage, L"Fatal" );
          //throw std::runtime_error( "Failed to create Win32 window!" );
        }

        return hWnd;
      }

    }//namespace detail
  }//namespace ui
}//namespace ds
