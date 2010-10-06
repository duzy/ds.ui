
#ifndef __DS_RESOURCE_ITEM__HPP____by_Duzy_Chan__
#define __DS_RESOURCE_ITEM__HPP____by_Duzy_Chan__ 1
#	include <string>
//#	include <boost/scoped_ptr.hpp>
#   include "../visibility.h"

namespace ds { namespace resource {

    struct DS_UI_PUBLIC item
    {
      typedef char byte_t;

      explicit item( const std::string & sName );
      virtual ~item();

      bool is_valid() const;

      const std::string & name() const;

      const byte_t * data() const;
      const byte_t * begin() const { return data(); }
      const byte_t * end() const { return data()+size(); }

      std::size_t size() const;

    private:
      struct impl;
      impl * _p;
    };//struct item

    /**
     *  @brief Register resource
     *
     *  @code
     *  const char * g_data[] = { ... };
     *  ds::resource::register_resource( ":/data/foo.dat", g_data, sizeof(g_data) );
     *  ds::resource::item res( ":/data/foo.dat" );
     *  std::clog<<res.size()<<std::endl;
     *  @endcode
     */

    DS_UI_PUBLIC
    bool register_resource( const std::string & name, const item::byte_t * d, std::size_t sz );

    enum {
      compiled_resource_version_1 = 0x00000001,
      item_type_file    = 0xA1,
      item_type_link    = 0xA2,
      item_type_alias   = 0xA3, //!< <ds:alias target="foo">a</ds:alias>
    };
    DS_UI_PUBLIC
    bool register_compiled_resource( const item::byte_t * const d, std::size_t sz );

  }//namespace resource
}//namespace ds

#define DS_RESOURCE_REGISTER_COMPILED(name)                             \
  namespace ds { namespace resource { namespace detail {                \
        static struct compiled_resource_register_class____##name        \
        {                                                               \
          inline compiled_resource_register_class____##name()           \
          {                                                             \
            if (!register_compiled_resource((const char*)name, sizeof(name))) { \
              dsE("can't register compiled resource: "#name);           \
            }                                                           \
          }                                                             \
        } g_compiled_resource_register____##name##__var;                \
      }                                                                 \
    }                                                                   \
  }


#endif//__DS_RESOURCE_ITEM__HPP____by_Duzy_Chan__
