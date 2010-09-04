/**
 *    Copyright 2010-07-18 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/
#ifndef __DS_UI_WINDOW_HPP____by_Duzy_Chan__
#define __DS_UI_WINDOW_HPP____by_Duzy_Chan__ 1
#       include <ds/shared_object.hpp>
#       include <ds/ui/events.hpp>
#       include <ds/ui/view.hpp>
//#       include <ds/graphics/rect.hpp>
#       include <ds/dso_visibility.hpp>

namespace ds { namespace graphics {
    struct box;
    class canvas;
  }//namespace graphics
}//namespace ds

namespace ds { namespace ui {

    class display;
    class screen;

    class DSO_PUBLIC window : public shared_object<window>
                            , public view
    {
      struct IMPL;

      friend class display;
      friend class screen;

    public:
      /**
       *  @brief Construct a trivial window.
       */
      window();

      /**
       *  @brief Construct a window in the display 'disp' and be ready for
       *         showing.
       */
      explicit window( const shared_object<display>::pointer & disp );

      ~window();

      shared_object<screen>::pointer get_screen() const;

      void select_input(long mask);

      void destroy();

      void show();
      void hide();
      void close();

      void move( int x, int y );
      void resize( int w, int h );

      //graphics::irect rect() const;
      graphics::box rect() const;

    public:
      struct on;

    protected:
      virtual void on_shown( const event::window::shown & );
      virtual void on_hidden( const event::window::hidden & );
      virtual void on_exposed( const event::window::exposed & );
      virtual void on_moved( const event::window::moved & );
      virtual void on_resized( const event::window::resized & );
      virtual void on_minimized( const event::window::minimized & );
      virtual void on_maximized( const event::window::maximized & );
      virtual void on_restored( const event::window::restored & );
      virtual void on_enter( const event::window::enter & );
      virtual void on_leave( const event::window::leave & );
      virtual void on_focus( const event::window::focus & );
      virtual void on_close( const event::window::close & );

      void do_close();

      virtual void on_render( ds::graphics::canvas & canvas );

    private:
      IMPL * _p;
    };//class window

  }//namespace ui
}//namespace ds

#endif//__DS_UI_WINDOW_HPP____by_Duzy_Chan__
