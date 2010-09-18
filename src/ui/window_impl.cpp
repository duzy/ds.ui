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
      // 
      // TODO: think of a better algorithm for updating the dirty rects
      // 
      box_list_t::const_iterator it = _dirty_rects.begin();
      for (; it != _dirty_rects.end(); ++it) {
        if ( it->intersects(dirty) ) {
          // TODO: ...
        }
        else if ( it->contains(dirty) ) {
          return; //!< just ignore the request because it's been dirty
        }
      }

      // TODO: _dirty_rects should be sorted, not just 'push_back' here
      _dirty_rects.push_back( dirty );
    }

    /**
     *
     */
    void window::IMPL::redraw_dirties( const display::pointer &, const window::pointer & win )
    {
      // TODO:(1) redraw the dirty rects, and mark each as _pended_updates

      if ( _dirty_rects.empty() ) {
        dsL5("nothing dirty");
        return;
      }

      const graphics::box wr( get_rect() ); // TODO: get_client_rect()
      if ( wr.is_empty() ) {
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

      box_list_t::const_iterator it = _dirty_rects.begin();
      for ( ; it != _dirty_rects.end(); ++it ) {
        dsL5("redraw: ["<<it->x()<<","<<it->y()<<","<<it->width()<<","<<it->height()<<"]");

        //TODO: dispatch drawing to widgets

        //canvas.clip( *it );
        win->on_render( canvas );
        //canvas.restore_clip();

        pend_update( *it );
      }

      _dirty_rects.clear(); //!< the window is cleaned
    }

    /**
     *  @see window::IMPL::commit_updates()
     */
    void window::IMPL::pend_update( const ds::graphics::box & b )
    {
      // TODO: same algorithm as set_dirty
      dsL5("mark-updated: ["<<b.x()<<","<<b.y()<<","<<b.width()<<","<<b.height()<<"]");
      _pended_updates.push_back( b );

      //commit_updates();
    }

  }//namespace ui
}//namespace ds

