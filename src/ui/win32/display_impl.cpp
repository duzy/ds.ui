/**
 *    Copyright 2010-09-12 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#define WIN32_LEAN_AND_MEAN
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
      if ( s_WindowClassName == NULL && regIfNon ) {
        s_WindowClassName = L"ds::ui::window.class";

        detail::window_class_register wc( s_WindowClassName, wndproc );

        bool ok = wc();
        dsI( ok );
      }
      return s_WindowClassName;
    }

    void display::IMPL::open_natively( const display::pointer & disp, const char * name )
    {
      dsI( !_scrns );
      dsI( _winmap.empty() );

      _scrns = new screen::pointer [1];
      _scrns[0].reset( new screen );
      _scrns[0]->_p->_display = disp;
    }

    bool display::IMPL::is_win_mapped_natively( const window::pointer & win )
    {
      dsI( win->_p );
      return ( win->_p->_native_win != NULL );
    }

    bool display::IMPL::map_win_natively( const window::pointer & win )
    {
      dsI( win->_p );
      return ( win->_p->_native_win != NULL );
    }

    bool display::IMPL::unmap_win_natively( const window::pointer & win )
    {
      return false;
    }

    /*
    HWND display::IMPL::default_root() const
    {
      const int dsn = default_screen_number();
      dsI( _scrns );
      dsI( 0 < screen_count() );
      dsI( 0 <= dsn && dsn < screen_count() );
      return _scrns[ dsn ]->root();
    }
    */

    bool display::IMPL::is_default_root( const window::pointer & w ) const
    {
      // TODO: ...
      return true;
    }
    
    int display::IMPL::screen_count() const
    {
      return 1;
    }

    int display::IMPL::default_screen_number() const
    {
      return 0;
    }

    void display::IMPL::pump_native_events( event_queue * eq )
    {
      MSG msg;
      while (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
        ::TranslateMessage( &msg );

        if (msg.message == WM_QUIT) {
          /**
           *  NOTE: after all windows is closed, the quit event will also
           *        be pushed in the WM_DESTROY handler
           */
          eq->push(new ds::event::quit);
          break;
        }

        ::DispatchMessage( &msg );
      }//while
    }

    LRESULT CALLBACK display::IMPL::wndproc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
      /**
       *  The CreateWindowEx sends WM_NCCREATE, WM_NCCALSIZE, WM_CREATE.
       *
       *  FIXME: any message sent before WM_NCCREATE will be losed.
       */
      if ( msg == WM_NCCREATE ) {
        //!< @see http://msdn.microsoft.com/en-us/library/ms632680(VS.85).aspx
        CREATESTRUCT * cs = reinterpret_cast<CREATESTRUCT*>( lParam );
        dsI( cs );
        dsI( cs->lpCreateParams );
        ::SetWindowLong( hWnd, GWL_USERDATA, reinterpret_cast<LONG>(cs->lpCreateParams) );
      }

      if ( LONG ud = ::GetWindowLong( hWnd, GWL_USERDATA ) ) {

        // TODO: think about this?
        display * d = reinterpret_cast<display*>(ud);

        if ( d->_p->push_event( d->get_queue(), hWnd, msg, wParam, lParam ) )
          return 0;
      } else {
        dsE( "missed: ("<<hWnd<<") "<<msg );
      }

      // NOTE: All other messages must be handled by DefWindowProc, or the
      //       CreateWindowEx will return NULL for a HWND.
      return ::DefWindowProc( hWnd, msg, wParam, lParam );
    }

  }//namespace ui
}//namespace ds
