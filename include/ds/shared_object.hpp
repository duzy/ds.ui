/**
 *    Copyright 2010-07-27 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#ifndef __DS_SHARED_OBJECT_HPP____by_Duzy_Chan__
#define __DS_SHARED_OBJECT_HPP____by_Duzy_Chan__ 1
#       include <boost/smart_ptr/intrusive_ptr.hpp>
#       include <boost/smart_ptr/detail/atomic_count.hpp>
#       include <boost/assert.hpp>
#       include <ds/shared_object_fwd.hpp>

namespace ds {

  namespace detail
  {
    template<class SO>
    class shared_object_base
    {
      template<class T> friend void intrusive_ptr_add_ref( shared_object_base<T> * );
      template<class T> friend void intrusive_ptr_release( shared_object_base<T> * );

      //typedef boost::detail::atomic_count atomic_word;
      typedef long atomic_word;
      atomic_word _usecount;
      atomic_word * _weakcount;

      void _inref()
      {
        ++_usecount;
      }

      void _deref()
      {
        if ( --_usecount == 0 ) {
          if (_weakcount != NULL) *_weakcount = -(*_weakcount);
          delete this;
        }
      }

    protected:
      shared_object_base() : _usecount( 0 ), _weakcount(NULL) {}

      virtual ~shared_object_base() {}

      /**
       *  Prevent 'this' to be deleted while converting a 'this' into a
       *  shared_object pointer, because the _usecount maybe zero.
       */
      class this_locker
      {
        shared_object_base * _so;
      public:
        explicit this_locker(shared_object_base * so) : _so(so)
        {
          ++so->_usecount;
        }

        ~this_locker()
        {
          BOOST_ASSERT(0 < _so->_usecount);
          --_so->_usecount;
        }
      };//struct this_locker

      class weak_ref_base
      {
        shared_object_base * _so;
        atomic_word * _weakcount;

      protected:
        SO * so() const {
          if ( _weakcount == NULL ) return NULL;
          if ( (*_weakcount) < 0 ) return NULL;
          return reinterpret_cast<SO*>(_so);
        }

      public:
        explicit weak_ref_base(shared_object_base * so)
          : _so(so), _weakcount(NULL)
        {
          if (so == NULL) return;
          if ((_weakcount = so->_weakcount) == NULL) {
            _weakcount = so->_weakcount = new atomic_word(1);
          }
          else {
            ++(*_weakcount);
          }
        }

        ~weak_ref_base()
        {
          if (_weakcount == NULL) return;

          // TODO: rethink about multi-thread environment
          if ( (*_weakcount) < 0 ) {
            if ( ++(*_weakcount) == 0 ) {
              delete _weakcount;
              _weakcount = NULL;
            }
          } else {
            if ( --(*_weakcount) == 0 ) {
              delete _weakcount;
              _weakcount = _so->_weakcount = NULL;
            }
          }
        }

        void swap( weak_ref_base & o )
        {
          shared_object_base * so = o._so;
          atomic_word * weakcount = o._weakcount;
          o._so = _so;
          o._weakcount = _weakcount;
          _so = so;
          _weakcount = weakcount;
        }

        void reset()
        {
          weak_ref_base(NULL).swap( *this );
        }

        long use_count() const
        {
          if (_weakcount == NULL) return 0;
          if ((*_weakcount) < 0) return 0;
          return _so->_usecount;
        }

        long weak_count() const
        {
          if (_weakcount == NULL) return 0;

          long wc(*_weakcount);
          if (wc < 0) return -wc;
          return wc;
        }
      };//class weak_ref_base

    public:
      long use_count() const
      {
        //return static_cast<int const volatile &>( _usecount );
        return ( _usecount );
      }

      /*
      long weak_count() const
      {
        return _weakcount ?  static_cast<int const volatile &>( *_weakcount )
          : 0 ;
      }
      */
    };//class shared_object_base
  }//namespace detail

  /**
   *  @brief Shared object with smart pointer.
   */
  template<class SO>
  struct shared_object : public detail::shared_object_base<SO>
  {
    /**
     *  Must be used to wrap raw pointers returned by 'new'. 
     */
    typedef boost::intrusive_ptr<SO> pointer;

    /**
     *  Use weak_ref to break cycles.
     */
    struct weak_ref : detail::shared_object_base<SO>::weak_ref_base
    {
      weak_ref()
        : detail::shared_object_base<SO>::weak_ref_base(NULL) {}

      /*explicit*/ weak_ref( const pointer & p )
        : detail::shared_object_base<SO>::weak_ref_base(p.get()) {}

      weak_ref( const weak_ref & r )
        : detail::shared_object_base<SO>::weak_ref_base(r.so()) {}

      weak_ref & operator=( const weak_ref & o )
      {
        weak_ref(o).swap( *this );
        return *this;
      }

      pointer lock() const { return pointer(this->so()); }
    };//struct weak_ref
  };//struct shared_object

}//namespace ds

#endif//__DS_SHARED_OBJECT_HPP____by_Duzy_Chan__
