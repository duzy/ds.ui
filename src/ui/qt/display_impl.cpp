/**
 *    Copyright 2010-07-25 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/
#include <ds/ui/display.hpp>
#include <ds/ui/screen.hpp>
#include <ds/ui/window.hpp>
#include <ds/ui/events.hpp>
#include <ds/event_queue.hpp>
#include <ds/debug.hpp>
#include <sys/types.h>
#include "../window_impl.h"
#include "../screen_impl.h"
#include "../display_impl.h"

namespace ds { namespace ui {

    class qapp : public QApplication
    {
    public:
      qapp( const display::pointer & disp, int & argc, char ** argv )
        : QApplication( argc, argv )
        , _display( disp )
      {
        dsI( disp );
      }

      bool notify(QObject *receiver, QEvent *event);

    protected:
      bool event( QEvent * );

    private:
      display::weak_ref _display;
    };

    bool qapp::event( QEvent * e )
    {
      dsL("event: "<<e->type());
      return QApplication::event(e);
    }

    bool qapp::notify(QObject *receiver, QEvent *e)
    {
      //dsL("notify: "<<e->type());
      display::pointer disp = _display.lock();                  dsI( disp );
      if ( display::IMPL::push_event( disp, receiver, e ) )
        return true;
      return QApplication::notify(receiver, e);
    }

    display::IMPL::IMPL()
      : _app( NULL )
    {
    }

    display::IMPL::~IMPL()
    {
      delete [] _scrns;
      delete _app;
    }

    void display::IMPL::open_natively( const display::pointer & disp, const char * name )
    {
      dsI( _winmap.empty() );
      dsI( _app == NULL );

      const int scrnCount = 1;
      _scrns = new screen::pointer [scrnCount];
      for ( int n = 0; n < scrnCount; ++n ) {
        screen::pointer s( new screen );
        s->_p->_display = disp;
        _scrns[n] = s;
      }

      int argc = 0;
      /*
      char * argv[] = {
        "/home/duzy/.v/out/debug/bin/simple",
        NULL,
      };
      _app = new qapp(argc, argv);
      */
      _app = new qapp(disp, argc, NULL);

      _winmap.clear();
    }

    void display::IMPL::pump_native_events( event_queue * /*eq*/ )
    {
      dsI( _app );

      _app->processEvents( QEventLoop::AllEvents );
    }

    bool display::IMPL::map_win_natively( const window::pointer & win )
    {
      return win->_p->_native_win != NULL;
    }

    bool display::IMPL::unmap_win_natively( const window::pointer & win )
    {
      return false;
    }

    bool display::IMPL::is_win_mapped_natively( const window::pointer & win )
    {
      return (win->_p->_native_win);
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

  }//namespace ui
}//namespace ds
