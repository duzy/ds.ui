/**
 *    Copyright 2010-09-12 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/ui/display.hpp>
#include <ds/ui/screen.hpp>
#include <ds/ui/window.hpp>
#include <ds/debug.hpp>
#include "../window_impl.h"
#include "../screen_impl.h"
#include "../display_impl.h"
#include "window_class_register.h"

namespace ds { namespace ui {

    display::IMPL::IMPL()
      : _scrns( NULL )
      , _winmap()
    {
    }

    display::IMPL::~IMPL()
    {
      delete [] _scrns;
    }

    const wchar_t * display::IMPL::get_window_class_name( bool regIfNon )
    {
      static const wchar_t * s_WindowClassName = NULL;
      if ( s_WindowClassName == NULL && regIfNon )  {
        s_WindowClassName = L"ds::ui::window";

        detail::window_class_register wc( s_WindowClassName, wndproc );

        bool ok = wc();
        dsI( ok );
      }
      return s_WindowClassName;
    }

    void display::IMPL::open( const display::pointer & disp, const char * name )
    {
      dsI( !_scrns );
      dsI( _winmap.empty() );

      _scrns = new screen::pointer [1];
      _scrns[0].reset( new screen );
      _scrns[0]->_p->_display = disp;
    }

    bool display::IMPL::map_win_natively( const window::pointer & win )
    {
      // TODO: ???
      return true;
    }

    bool display::IMPL::unmap_win_natively( const window::pointer & win )
    {
      return false;
    }

    bool display::IMPL::is_win_mapped_natively( const window::pointer & win )
    {
      return (win->_p->_native_win);
    }

    HWND display::IMPL::default_root() const
    {
      return NULL;
    }

    bool display::IMPL::is_default_root( const window::pointer & w ) const
    {
      return false;
    }
    
    int display::IMPL::screen_count() const
    {
      return 1;
    }

    int display::IMPL::default_screen_number() const
    {
      return 0;
    }

    screen::pointer display::IMPL::get_screen( int n ) const
    {
      return NULL;
    }

    void display::IMPL::pump_native_events( event_queue * eq )
    {
      MSG msg;
      //memset( &msg, 0, sizeof(msg) );

      while (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
        ::TranslateMessage( &msg );

        if (msg.message == WM_QUIT) {
          // TODO: post quit message
          break;
        }

        ::DispatchMessage( &msg );
      }
    }

    LRESULT CALLBACK display::IMPL::wndproc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
      LONG ud = ::GetWindowLong( hWnd, GWL_USERDATA );

      if ( ud == 0 )
        return 0;

      // TODO: think about this?
      display * d = reinterpret_cast<display*>(ud);

      d->_p->push_event( d->get_queue(), hWnd, msg, wParam, lParam );
      
      return 0;
    }

  }//namespace ui
}//namespace ds
