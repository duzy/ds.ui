#include <ds/ui/events.hpp>
#include <ds/ui/display.hpp>
#include <ds/event_queue.hpp>

namespace ds { namespace ui {

    event_queue * get_event_queue()
    {
      static event_queue q;
      return &q;
    }

    event_loop::event_loop(const display::pointer_t & disp)
      : ds::event_loop( get_event_queue() )
    {
      //disp->start_pump();
    }

    event_loop::~event_loop()
    {
    }

    void event_loop::should_pump_events()
    {
      // TODO: pump events from the display
    }

    void event_loop::on_event(const ds::event & evt)
    {
      // TODO: dispatch UI events
      ds::event_loop::on_event(evt);
    }
    
  }//namespace ui
}//namespace ds
