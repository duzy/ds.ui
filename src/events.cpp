#include <ds/ui/events.hpp>
#include <ds/ui/display.hpp>

namespace ds { namespace ui {

    static event_queue * get_event_queue()
    {
      static event_queue q;
      return &q;
    }

    event_loop::event_loop(const display::pointer_t & disp)
      : ds::event_loop( get_event_queue() )
    {
      disp->start_pump();
    }

    event_loop::~event_loop()
    {
    }

    void event_loop::on_event(const event & evt)
    {
      // TODO: dispatch UI events
      ds::event_loop::on_event(evt);
    }
    
  }//namespace ui
}//namespace ds
