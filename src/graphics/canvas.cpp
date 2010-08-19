#include <ds/graphics/canvas.hpp>
#include <ds/graphics/gil/image.hpp>
namespace ds { namespace graphics {

    canvas::canvas( gil::image & img )
      : _d( &img )
      , _isView( 0 )
    {
    }

    canvas::canvas( gil::view & v )
      : _v( &v )
      , _isView( 1 )
    {
    }

  }//namespace graphics
}//namespace ds
