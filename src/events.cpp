#include <ds/event.hpp>
#include <ds/ui/events.hpp>
#include <ds/ui/display.hpp>
#include <ds/event_queue.hpp>
#include <ds/debug.hpp>

namespace ds { namespace ui {

    event_queue * get_event_queue()
    {
      static event_queue q;
      return &q;
    }

    event_loop::event_loop(const display::pointer_t & disp)
      : ds::event_loop( get_event_queue() )
      , _display( disp )
    {
      //disp->start_pump();
    }

    event_loop::~event_loop()
    {
    }

    void event_loop::should_pump_events()
    {
      // TODO: pump events from the display
      _display->pump();
    }

    void event_loop::on_event(const ds::event & evt)
    {
      // TODO: dispatch UI events
      dsD("event: "<<evt.type);
      ds::event_loop::on_event(evt);
    }
    
  }//namespace ui
}//namespace ds
