/**
 *    Copyright 2010-07-25 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <map>
//#include <ext/slist>
#include <list>

namespace ds { namespace ui {

    class window;
    class screen;

    struct display::IMPL
    {
      typedef std::map<native_window_t, shared_object<window>::pointer> window_map_t;
      //typedef __gnu_cxx::slist<window::pointer> dirty_wins_t;
      typedef std::list<window::pointer> dirty_wins_t;

      IMPL();
      ~IMPL();

      void open( const display::pointer & disp, const char * name );
      void pump_native_events( event_queue * );
      bool map_win_natively( const shared_object<window>::pointer & win );
      bool unmap_win_natively( const shared_object<window>::pointer & win );
      bool is_win_mapped_natively( const shared_object<window>::pointer & win );
      int screen_count() const;
      int default_screen_number() const;
      shared_object<screen>::pointer get_screen( int n ) const;
      native_window_t default_root() const;
      bool is_default_root( const shared_object<window>::pointer & w ) const;

      void set_win_dirty( const window::pointer & );
      void redraw_dirty_wins( const display::pointer & );

      dirty_wins_t _dirty_wins;
      
      shared_object<screen>::pointer * _scrns;
      window_map_t _winmap;

#ifdef _WIN32
      
      static const wchar_t * get_window_class_name( bool regIfNon = false );
      static LRESULT CALLBACK wndproc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
      bool push_event( event_queue *, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

#elif defined(X11)

      Display * _xdisplay;
      Atom WM_DELETE_WINDOW;
      void init_atoms();
      bool pending();
      void push_event( event_queue *, XEvent * event );

#else
#   error unsupported platform
#endif
    };//struct display::IMPL

  }//namespace ui
}//namespace ds
