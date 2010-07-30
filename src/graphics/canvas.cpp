#include <ds/graphics/canvas.hpp>
#include <ds/graphics/gil/image.hpp>
#include <ds/graphics/image.hpp>

namespace ds { namespace graphics {

    canvas::canvas( image & img )
      : _image( &img )
    {
    }

    void canvas::clip( const irect & )
    {
      // TODO: ...
    }

  }//namespace graphics
}//namespace ds
