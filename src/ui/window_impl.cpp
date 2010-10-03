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
#include <ds/ui/window.hpp>
#include <ds/ui/screen.hpp>
#include <ds/ui/display.hpp>
#include "window_impl.h"
#include <ds/debug.hpp>
#include <ds/graphics/canvas.hpp>
#include <boost/geometry/algorithms/make.hpp>

namespace ds { namespace ui {

    /**
     *  Mark a area as dirty which is required for renderring.
     *
     *  @see window::request_update
     */
    void window::IMPL::set_dirty( const ds::graphics::box & dirty )
    {
      // (1) add dirty rect into the _dirty_rects list(sorted)
      _dirty_region |= dirty;
    }

    /**
     *  @see window::IMPL::commit_updates()
     */
    void window::IMPL::pend_update( const ds::graphics::box & b )
    {
      dsL5("mark-updated: ["<<b.x()<<","<<b.y()<<","<<b.width()<<","<<b.height()<<"]");
      _pended_updates |= b;
    }

    /**
     *
     */
    void window::IMPL::redraw_dirties( const display::pointer &, const window::pointer & win )
    {
      // TODO:(1) redraw the dirty rects, and mark each as _pended_updates

      if ( _dirty_region.empty() ) {
        dsL5("nothing dirty");
        return;
      }

      const graphics::box wr( get_rect() ); // TODO: get_client_rect()
      if ( wr.empty() ) {
        dsE("empty window rect: "<<_native_win<<", "<<wr.x()<<","<<wr.y()<<","<<wr.width()<<","<<wr.height());
        return;
      }

      if ( !create_image_if_needed( wr.width(), wr.height() ) ) {
        dsE("Failed to create renderring buffer");
        return;
      }

      dsI( _image.is_valid() );
      dsI( _image.pixels() );
      dsI( 0 < _image.width() );
      dsI( 0 < _image.height() );

      ds::graphics::canvas canvas( _image );
      ds::graphics::box bound = _dirty_region.bounds();
      ds::graphics::region::const_iterator it = _dirty_region.begin();
      
      for ( ; it != _dirty_region.end(); ++it ) {
        dsL5("redraw: ["<<it->x()<<","<<it->y()<<","<<it->width()<<","<<it->height()<<"]");

        //TODO: dispatch drawing to widgets

        //canvas.clip( *it );
        win->on_render( canvas );
        //canvas.restore_clip();

        pend_update( *it );
      }

      _dirty_region.clear(); //!< the window is cleaned
    }

    /**
     *  Put all dirty rects onto the screen.
     *
     *  @return true if any dirty rects has been push onto the screen.
     */
    bool window::IMPL::commit_updates()
    {
      if ( _pended_updates.empty() ) {
        dsL4("window is updated");
        return false;
      }

      dsL5("commit-updates: "<<_pended_updates.size()-1);

      ds::graphics::box const & bound = _pended_updates.bounds();
      if ( bound.empty() ) {
        dsE("empty update bounds");
        return false;
      }

      ds::graphics::region::const_iterator it = _pended_updates.begin();
      ds::graphics::region::const_iterator const end = _pended_updates.end();

      //_pended_updates.dump( std::cout << "updates: " );

      int count = 0;
      for (; it != end; ++it) {
        if ( commit_update_natively( *it ) ) ++count;
      }

      if ( 0 < count ) {
        sync_updates_natively();
      }

      _pended_updates.clear();
      return _pended_updates.empty();
    }

  }//namespace ui
}//namespace ds

