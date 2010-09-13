/**
 *    Copyright 2010-05-25 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/
#ifndef __DS_UI_DISPLAY_HPP____by_Duzy_Chan__
#define __DS_UI_DISPLAY_HPP____by_Duzy_Chan__ 1
#       include <boost/noncopyable.hpp>
#       include <ds/shared_object.hpp>
#       include <ds/event_pump.hpp>
#       include <ds/dso_visibility.hpp>

namespace ds { namespace ui {

    class screen;
    class window;
    class event_loop;

    /**
     *  @brief Windows is living in a particular screen of a display.
     *
     *  @usage
     *  @code
     *    class MyLoop
     *      : public ds::ui::event_loop
     *    {
     *    public:
     *      MyLoop( const ds::ui::display::pointer & disp )
     *        : ds::ui::event_loop( disp )
     *      {
     *      }
     *
     *    protected:
     *      virtual void on_event( const event & evt)
     *      {
     *        // TODO: hack into the event loop here...
     *        ds::ui::event_loop::on_event(evt);
     *      }
     *    };
     *    
     *    int main()
     *    {
     *      ds::ui::display::pointer disp( ds::ui::display::open() );
     *      
     *      MyLoop loop( disp );
     *      return loop.run();
     *    }
     *  @endcode
     */
    class DSO_PUBLIC display
      : boost::noncopyable
      , public shared_object<display>
      , public event_pump
    {
      display();
      ~display();

      struct IMPL;

      friend class screen;
      friend class window;
      friend class event_loop;

    public:
      class id
      {
        void * _p;

        friend class display;

      public:
        id(void * p = 0) : _p(p) {}
      };

      // TODO:
      //        1) move semantic,
      //        2) think about 'close' display
      static pointer open( id i = id() );

      shared_object<screen>::pointer default_screen() const;
      shared_object<screen>::pointer get_screen( int index ) const;
      int default_screen_number() const;
      int screen_count() const;

      shared_object<window>::pointer default_root() const;

      void map( const shared_object<window>::pointer& win );
      void unmap( const shared_object<window>::pointer& win );
      bool has( const shared_object<window>::pointer& win );

    protected:
      void start( event_queue * q ) { this->set_queue( q ); }

      virtual void pump_events(); //! must be thread safe
                                  //! must be the same thread as invoking 'open'
                                  //! must be the same thread as invoking 'start_pump'


    private:
      IMPL * _p;
    };//class display
    
  }//namespace ui
}//namespace ds

#endif//__DS_UI_DISPLAY_HPP____by_Duzy_Chan__
