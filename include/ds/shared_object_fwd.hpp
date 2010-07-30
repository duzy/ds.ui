/**
 *    Copyright 2010-07-27 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#ifndef __DS_SHARED_OBJECT_FWD_HPP____by_Duzy_Chan__
#define __DS_SHARED_OBJECT_FWD_HPP____by_Duzy_Chan__ 1
#define DS_SHARED_OBJECT(TYPE)                                          \
  inline void intrusive_ptr_add_ref( TYPE * p )                         \
  {                                                                     \
    ds::detail::intrusive_ptr_add_ref(reinterpret_cast<ds::detail::shared_object_impl<TYPE>*>(p)); \
  }                                                                     \
  inline void intrusive_ptr_release( TYPE * p )                         \
  {                                                                     \
    ds::detail::intrusive_ptr_release(reinterpret_cast<ds::detail::shared_object_impl<TYPE>*>(p)); \
  }

namespace ds
{

  namespace detail
  {
    template<class SO> class shared_object_impl;
    template<class SO>
    inline void intrusive_ptr_add_ref( shared_object_impl<SO> * p )
    {
      //(void) sizeof(SO); // ensure complete type
      //(void) sizeof(shared_object_impl<SO>); // ensure complete type
      p->_inref();
    }

    template<class SO>
    inline void intrusive_ptr_release( shared_object_impl<SO> * p )
    {
      //(void) sizeof(SO); // ensure complete type
      //(void) sizeof(shared_object_impl<SO>); // ensure complete type
      p->_deref();
    }
  }//namespace detail

  template<class SO> struct shared_object;

}//namespace ds

#endif//__DS_SHARED_OBJECT_FWD_HPP____by_Duzy_Chan__
