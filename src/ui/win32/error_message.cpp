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
#include "error_message.h"

namespace ds { namespace ui { namespace detail {

      std::wstring get_last_error_message()
      {
        std::wstring errorMessage;
        DWORD errorCode = ::GetLastError();
        LPVOID error;
        if ( ::FormatMessageW( FORMAT_MESSAGE_ALLOCATE_BUFFER
                               | FORMAT_MESSAGE_FROM_SYSTEM,
                               NULL,
                               errorCode,
                               MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                               (LPWSTR)&error,
                               0, 0 ) ) {
          errorMessage = (LPWSTR)error;
          ::LocalFree( error );
        }
        return errorMessage;
      }

      void error_message_box( const std::wstring& err, const std::wstring& title )
      {
        ::MessageBoxW( NULL, err.c_str(), title.c_str(),
                       MB_ICONEXCLAMATION | MB_OK );
      }

    }//namespace detail
  }//namespace ui
}//namespace ds
