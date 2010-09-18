/**
 *    Copyright 2010-09-18 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#endif
#include <ds/ui/display.hpp>
#include <ds/ui/screen.hpp>
#include <ds/ui/window.hpp>
#include <ds/debug.hpp>
#include <algorithm>
#include "window_impl.h"
#include "display_impl.h"

namespace ds { namespace ui {

    void display::IMPL::set_win_dirty( const window::pointer & w )
    {
      dsI( !( w->_p->_dirty_rects.empty() ) );

      if ( std::find( _dirty_wins.begin(), _dirty_wins.end(), w ) == _dirty_wins.end() ) {
        dsL( "set-window-dirty: "<<w->_p->_native_win );
        _dirty_wins.push_back( w );
      }
    }

    void display::IMPL::redraw_dirty_wins( const display::pointer & disp )
    {
      dirty_wins_t::iterator it = _dirty_wins.begin();
      for (; it != _dirty_wins.end(); ++it) {
        dsL( "redraw-window: "<<(*it)->_p->_native_win );
        (*it)->_p->redraw_dirties( disp, *it );
        dsI( (*it)->_p->_dirty_rects.empty() );
        (*it)->_p->commit_updates();
      }

      _dirty_wins.clear(); //!< all windows are clean
    }

  }//namespace ui
}//namespace ds
