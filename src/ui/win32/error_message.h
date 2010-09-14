/**
 *    Copyright 2010-09-15 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

namespace ds { namespace ui { namespace detail {

      std::wstring get_last_error_message();

      void error_message_box( const std::wstring& err, const std::wstring& title );

    }//namespace detail
  }//namespace ui
}//namespace ds
