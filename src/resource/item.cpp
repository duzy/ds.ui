/**	                                                             -*- c++ -*-
 *  Copyright by Duzy Chan <code@duzy.info>
 */
#include <ds/resource/item.hpp>
#include <ds/resource/streambuf.hpp>
#include <ds/debug.hpp>
#include <map>

namespace ds { namespace resource {

    struct repository
    {
      typedef std::pair<const item::byte_t*, std::size_t> item_pair;
      typedef std::map<std::string, item_pair> items_t;

      static bool add( const std::string & name, const item::byte_t * d, std::size_t sz );
      static item_pair get( const std::string & name );

    private:
      static std::map<std::string, item_pair> & get_items();
    };//struct repository

    repository::items_t & repository::get_items()
    {
      static items_t items;
      return items;
    }

    bool repository::add( const std::string & name, const item::byte_t * d, std::size_t sz )
    {
      items_t & items( get_items() );
      std::map<std::string, item_pair>::const_iterator it(items.find(name));
      if ( it != items.end() ) return false;
      items.insert( std::make_pair(name, std::make_pair(d, sz)) );
      return true;
    }

    repository::item_pair repository::get( const std::string & name )
    {
      items_t & items( get_items() );
      std::map<std::string, item_pair>::const_iterator it(items.find(name));
      if ( it == items.end() ) return std::make_pair((const char*)nullptr, 0);
      return it->second;
    }

    //============================================================

    bool register_resource( const std::string & name, const item::byte_t * d, std::size_t sz )
    {
      return repository::add( name, d, sz );
    }

    bool register_compiled_resource( const item::byte_t * const d, std::size_t sz )
    {
      if ( sz < 16 ) {
        dsE("compiled resource data too short: "<<sz);
        return false;
      }

      int version( *reinterpret_cast<const int*>(d) );
      if ( version != compiled_resource_version_1 ) {
        dsE("bad version: "<<version);
        return false;
      }

      int indexOffset( *reinterpret_cast<const int*>(d + sz - sizeof(int)) );
      if ( sz <= (indexOffset + sizeof(int)) ) {
        dsE("index offset out of range: "<<indexOffset<<", size="<<sz);
        return false;
      }
      dsL4("index offset: "<<indexOffset);
      
      const item::byte_t * p( d + indexOffset );
      int count( *reinterpret_cast<const int*>(p) ); p += sizeof(int);
      if ( sz <= count ) {
        dsE("invalid index size: "<<count);
        return false;
      }
      dsL4("number of lists: "<<count);

      std::string prefix, name;
      register int off, nameLen, dataLen;
      const item::byte_t * pp;
      item::byte_t itemType( 0 );
      for(int n=0; n < count; ++n) {
        int len( *reinterpret_cast<const int*>(p) ); p += sizeof(int);
        if ( sz <= len ) {
          dsE("invalid string size: "<<len);
          return false;
        }

        prefix.assign( p, p + len ); p += len + 1/* char '\0' */;

        len = *reinterpret_cast<const int*>(p); p += sizeof(int);
        for( int i=0; i < len; ++i ) {
          off = ( *reinterpret_cast<const int*>(p) ); p += sizeof(int);

          itemType = *(pp = d + off); pp += 1;
          if ( item::byte_t(item_type_file) == itemType ) {
            nameLen = ( *reinterpret_cast<const int*>(pp) ); pp += sizeof(int);
            name.assign( pp, pp + nameLen ); pp += nameLen + 1/* '\0' */;
          }
          else if ( item::byte_t(item_type_link) == itemType ) {
            nameLen = ( *reinterpret_cast<const int*>(pp) ); pp += sizeof(int);
            name.assign( pp, pp + nameLen ); pp += nameLen + 1/* '\0' */;
            do {
              off = *reinterpret_cast<const int*>( pp );
              itemType = *(pp = d + off); pp += 1;
              switch ( itemType ) {
              case item::byte_t(item_type_file):
              case item::byte_t(item_type_link):
                nameLen = ( *reinterpret_cast<const int*>(pp) );
                pp += sizeof(int) + nameLen + 1/* '\0' */;
                break;
              default:
                dsE("unknown item type: "<<int(itemType));
                return false;
              }
            } while ( item::byte_t(item_type_link) == itemType );
          }
          else {
            dsE("unknown item type: "<<int(itemType));
            return false;
          }

          dataLen = ( *reinterpret_cast<const int*>(pp) ); pp += sizeof(int);
          if ( !register_resource( prefix + name, pp, dataLen ) ) {
            dsE("can't register_resource(): "<<prefix<<","<<name);
            return false;
          }
          dsL4("item: "<<prefix<<name<<", length="<<dataLen);
        }
      }//!< for()

      return true;
    }

    //======================================================================

    struct item::impl
    {
      std::string _name;
      const item::byte_t *_data;
      std::size_t _size;

      explicit impl( const std::string & );

      bool is_valid() const;
      const std::string & name() const;
      const item::byte_t * data() const;
      std::size_t size() const;
    };//struct item::impl

    item::impl::impl( const std::string & s )
      : _name( s )
      , _data( NULL )
      , _size( 0 )
    {
      repository::item_pair ip( repository::get( _name ) );
      _data = ip.first, _size = ip.second;
    }

    bool item::impl::is_valid() const
    {
      return _data != nullptr && 0 < _size;
    }

    const std::string & item::impl::name() const
    {
      return _name;
    }

    const item::byte_t * item::impl::data() const
    {
      return _data;
    }

    std::size_t item::impl::size() const
    {
      return _size;
    }

    //============================================================

    item::item( const std::string & sName )
      : _p( new impl(sName) )
    {
    }

    item::~item()
    {
      delete _p;
    }

    bool item::is_valid() const
    {
      return _p->is_valid();
    }

    const std::string & item::name() const
    {
      return _p->name();
    }

    const item::byte_t * item::data() const
    {
      return _p->data();
    }

    std::size_t item::size() const
    {
      return _p->size();
    }

  }//namespace resource
}//namespace ds
