#ifndef __DS_RESOURCE_STREAMBUF__HPP____by_Duzy_Chan__
#define __DS_RESOURCE_STREAMBUF__HPP____by_Duzy_Chan__ 1
#   include "item.hpp"
#   include <streambuf>
#   include <boost/noncopyable.hpp>
#   include <iostream>
#   include "../visibility.h"

#   ifndef HAS_KEYWORD__nullptr
//#	define nullptr ((void*)0)
#	define nullptr NULL
#   endif

namespace ds { namespace resource {

    /**
     *  @brief The read only stream buffer working with std::istream. 
     *  @code
     *  ds::resource::streambuf buf(":/images/a.png");
     *  std::istream is( &buf );
     *  image = load_image_from_istream( is );
     *  @endcode
     */
    struct DS_UI_PUBLIC streambuf : std::streambuf, private boost::noncopyable
    {
      //explicit streambuf( const item * );

      //!< e.g.: ':/images/a.png'
      explicit streambuf( const std::string & name )
        : std::streambuf()
        , _item( name )
      {
        char * const beg( const_cast<char*>(_item.begin()) );
        char * const end( const_cast<char*>(_item.end()) );
        setg( beg, beg, end );
        setp( end, end ); //!< readonly
      }

      bool is_valid() const { return _item.is_valid(); }

    protected:

      //== Override std::streambuf behaviour. ==
      /**
       *  Behaves according to std::streambuf::underflow().
       */
      virtual int_type underflow()
      {
        return traits_type::eof();
      }

      //== Override std::streambuf behaviour. ==
      /**
       *  Behaves according to std::streambuf::seekoff().
       */
      virtual pos_type seekoff( off_type off, std::ios_base::seekdir dir,
                                std::ios_base::openmode mode )
      {
        if ( !(mode & std::ios_base::in) ) {
          return pos_type(off_type(-1));
        }
        switch ( dir ) {
        case std::ios_base::beg:
          setg( eback(), eback() + off, egptr() );
          break;
        case std::ios_base::cur:
          setg( eback(), gptr() + off, egptr() );
          break;
        case std::ios_base::end:
          setg( eback(), egptr() + off, egptr() );
          break;
        }//switch
        return pos_type(off_type( gptr() - eback() ));
      }

      //== Override std::streambuf behaviour. ==
      /**
       *  Behaves according to std::streambuf::seekpos().
       */
      virtual pos_type seekpos( pos_type pos, std::ios_base::openmode mode )
      {
        if ( !(mode & std::ios_base::in) ) return pos_type(off_type(-1));
        if ( 0 != pos ) setg( eback(), eback() + pos, egptr() );
        return pos_type(off_type( gptr() - eback() ));
      }

    private:
      item _item;
    };//struct streambuf

  }//namespace resource
}//namespace ds

#endif//__DS_RESOURCE_STREAMBUF__HPP____by_Duzy_Chan__
