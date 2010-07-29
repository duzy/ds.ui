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

namespace ds {

  namespace detail
  {
    template<class DerivedClass> class shared_object_impl;

    template<class DerivedClass>
    inline void intrusive_ptr_add_ref( shared_object_impl<DerivedClass> * p )
    {
      p->_inref();
    }

    template<class DerivedClass>
    inline void intrusive_ptr_release( shared_object_impl<DerivedClass> * p )
    {
      p->_deref();
    }

    template<class DerivedClass>
    class shared_object_impl
    {
      template<class T> friend void intrusive_ptr_add_ref( shared_object_impl<T> * );
      template<class T> friend void intrusive_ptr_release( shared_object_impl<T> * );

      int _refcount;

      void _inref()
      {
        //if ( _refcount < 0 ) _refcount = 1;
        //else
          ++_refcount;
      }

      void _deref()
      {
        //if ( _refcount < 0 ) return;
        if ( --_refcount == 0 ) {
          delete this;
        }
      }

    protected:
      //shared_object_impl() : _refcount( -1 ) {}
      shared_object_impl() : _refcount( 0 ) {}

      virtual ~shared_object_impl() {}

    public:
      int refcount() const { return _refcount < 0 ? 0 : _refcount; }
    };//struct shared_object_impl

  }//namespace detail

  /**
   *  @brief Shared object with smart pointer.
   */
  template<class DerivedClass>
  struct shared_object : public detail::shared_object_impl<DerivedClass>
  {
    /**
     *  Must be used to wrap raw pointers returned by 'new'. 
     */
    typedef boost::intrusive_ptr<DerivedClass> pointer;
  };//struct shared_object

}//namespace ds

#endif//__DS_SHARED_OBJECT_HPP____by_Duzy_Chan__
