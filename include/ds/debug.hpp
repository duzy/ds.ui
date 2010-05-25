/**
 *    Copyright 2008-03-01 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id: debug.hpp 858 2010-01-02 06:38:53Z duzy $
 *
 **/

#ifndef __DEBUG_HPP____by_Duzy_Chan__
#define __DEBUG_HPP____by_Duzy_Chan__ 1
#
#  if defined(DS_TRACE_LEVEL) && !defined(ENABLE_DS_TRACE)
#    define ENABLE_DS_TRACE true
#  endif
#
#  if defined(DS_DEBUG_LEVEL) && !defined(ENABLE_DS_DEBUG)
#    define ENABLE_DS_DEBUG true
#  endif
#
#  if defined(DS_LOG_LEVEL) && !defined(ENABLE_DS_LOG)
#    define ENABLE_DS_LOG true
#  endif
#
#  ifndef ENABLE_DS_ASSERT
#    define ENABLE_DS_ASSERT true
#  endif
#
#  if defined(ENABLE_DS_TRACE) || defined(ENABLE_DS_LOG) || defined(ENABLE_DS_DEBUG)
#       include <iostream>
#  endif
#       include <boost/assert.hpp>
#

namespace
{
  struct ds_internal__debug
  {
#  ifdef BOOST_THREAD_MUTEX_HPP
    typedef boost::mutex::scoped_lock lock_t;
    static lock_t lock()
    {
      static boost::mutex m;
      return lock_t( m );
    }
#  else
    //#    warning "BOOST_THREAD_MUTEX_HPP not defined, ds::debug is not thread safty."
    typedef struct {} lock_t;
    static lock_t lock() { lock_t t; return t; }
#  endif
  };//struct ds_internal__debug
}//namespace

////////////////////////////////////////////////////////////////////////////////
//      Tracer
//  * dsT       trace the current function
//  * dsTs      trace a named scope
////////////////////////////////////////////////////////////////////////////////
#  if ENABLE_DS_TRACE
#       define dsT_define_tracer(F, L, FN, msg, Lv)                     \
  ds_internal__tracer_class var_invisible____ds_tracer____##L##_(F,L,FN,msg,Lv)
#       define dsT_(msg,Lv) dsT_define_tracer(__FILE__, __LINE__, __FUNCTION__, msg, Lv)
#       define dsTs_(S,msg,Lv) dsT_define_tracer(__FILE__, __LINE__, S, msg, Lv)
namespace {
  struct ds_internal__tracer_class
  {
    std::string file, msg, scope;
    int line, level;
    ds_internal__tracer_class( std::string const & _file, int _line,
                               std::string const & _scope,
                               std::string const & _msg, int _level )
      : file(_file), msg(_msg), scope(_scope), line(_line), level(_level)
    {
      ds_internal__debug::lock_t lk( ds_internal__debug::lock() );
      std::clog
        <<file<<":"
        <<line<<":"
        <<"info: (enter) "<<scope
        <<"\n\ttrace("<<level<<"): "<<msg
        <<std::endl
        ;
    }
    ~ds_internal__tracer_class()
    {
      ds_internal__debug::lock_t lk( ds_internal__debug::lock() );
      std::clog
        <<file<<":"
        <<line<<":"
        <<"info: (leave) "<<scope
        <<std::endl
        ;
    }
  };
}//namespace
#       ifndef DS_TRACE_LEVEL
#               define DS_TRACE_LEVEL 0
#       endif
#       define dsT0(msg) ((void)false)
#       define dsT1(msg) ((void)false)
#       define dsT2(msg) ((void)false)
#       define dsT3(msg) ((void)false)
#       define dsT4(msg) ((void)false)
#       define dsT5(msg) ((void)false)
#       define dsTs0(msg) ((void)false)
#       define dsTs1(msg) ((void)false)
#       define dsTs2(msg) ((void)false)
#       define dsTs3(msg) ((void)false)
#       define dsTs4(msg) ((void)false)
#       define dsTs5(msg) ((void)false)
#       if 5 <= DS_TRACE_LEVEL
#               undef  dsT5
#               undef  dsTs5
#               define dsT5(msg) dsT_(msg,5)
#               define dsTs5(S,msg) dsTs_(S,msg,5)
#       endif
#       if 4 <= DS_TRACE_LEVEL
#               undef  dsT4
#               undef  dsTs4
#               define dsT4(msg) dsT_(msg,4)
#               define dsTs4(S,msg) dsTs_(S,msg,4)
#       endif
#       if 3 <= DS_TRACE_LEVEL
#               undef  dsT3
#               undef  dsTs3
#               define dsT3(msg) dsT_(msg,3)
#               define dsTs3(S,msg) dsTs_(S,msg,3)
#       endif
#       if 2 <= DS_TRACE_LEVEL
#               undef  dsT2
#               undef  dsTs2
#               define dsT2(msg) dsT_(msg,2)
#               define dsTs2(S,msg) dsTs_(S,msg,2)
#       endif
#       if 1 <= DS_TRACE_LEVEL
#               undef  dsT1
#               undef  dsTs1
#               define dsT1(msg) dsT_(msg,1)
#               define dsTs1(S,msg) dsTs_(S,msg,1)
#       endif
#       if 0 == DS_TRACE_LEVEL
#               undef  dsT0
#               undef  dsTs0
#               define dsT0(msg) dsT_(msg,0)
#               define dsTs0(S,msg) dsTs_(S,msg,0)
#       endif
#       define dsT(msg) dsT3(msg)
#       define dsTs(S,msg) dsTs3(S,msg)
#  else
#       define dsT(msg) ((void)false)
#       define dsT0(msg) ((void)false)
#       define dsT1(msg) ((void)false)
#       define dsT2(msg) ((void)false)
#       define dsT3(msg) ((void)false)
#       define dsT4(msg) ((void)false)
#       define dsT5(msg) ((void)false)
#       define dsTs(msg) ((void)false)
#       define dsTs0(msg) ((void)false)
#       define dsTs1(msg) ((void)false)
#       define dsTs2(msg) ((void)false)
#       define dsTs3(msg) ((void)false)
#       define dsTs4(msg) ((void)false)
#       define dsTs5(msg) ((void)false)
#  endif
//______________________________________________________________________________


////////////////////////////////////////////////////////////////////////////////
//      Debuger
//  There are six level debug message, higher level means more debug outputs:
//  * dsD0
//  * dsD1
//  * dsD2
//  * dsD3
//  * dsD4
//  * dsD5
//  The default debuger 'dsD' is set to level 3
////////////////////////////////////////////////////////////////////////////////
#  if ENABLE_DS_DEBUG
#       define dsD_(msg,lv)                                         \
  do {                                                              \
    ds_internal__debug::lock_t lk( ds_internal__debug::lock() );    \
    std::clog                                                       \
      <<__FILE__<<":"                                               \
      <<__LINE__<<":"                                               \
      <<"debug("<<lv<<"): "<<msg                                    \
      <<", in "<<__FUNCTION__                                       \
      <<std::endl;                                                  \
  } while(0)

#       ifndef DS_DEBUG_LEVEL
#               define DS_DEBUG_LEVEL 0
#       endif
#       define dsD0(msg) ((void)false)
#       define dsD1(msg) ((void)false)
#       define dsD2(msg) ((void)false)
#       define dsD3(msg) ((void)false)
#       define dsD4(msg) ((void)false)
#       define dsD5(msg) ((void)false)
#       define dsD0if(cond,msg) ((void)false)
#       define dsD1if(cond,msg) ((void)false)
#       define dsD2if(cond,msg) ((void)false)
#       define dsD3if(cond,msg) ((void)false)
#       define dsD4if(cond,msg) ((void)false)
#       define dsD5if(cond,msg) ((void)false)
#       if 5 <= DS_DEBUG_LEVEL
#               undef  dsD5
#               undef  dsD5if
#               define dsD5(msg) dsD_(msg,5)
#               define dsD5if(cond,msg) if (cond) { dsD5(msg); }
#       endif
#       if 4 <= DS_DEBUG_LEVEL
#               undef  dsD4
#               undef  dsD4if
#               define dsD4(msg) dsD_(msg,4)
#               define dsD4if(cond,msg) if (cond) { dsD4(msg); }
#       endif
#       if 3 <= DS_DEBUG_LEVEL
#               undef  dsD3
#               undef  dsD3if
#               define dsD3(msg) dsD_(msg,3)
#               define dsD3if(cond,msg) if (cond) { dsD3(msg); }
#       endif
#       if 2 <= DS_DEBUG_LEVEL
#               undef  dsD2
#               undef  dsD2if
#               define dsD2(msg) dsD_(msg,2)
#               define dsD2if(cond,msg) if (cond) { dsD2(msg); }
#       endif
#       if 1 <= DS_DEBUG_LEVEL
#               undef  dsD1
#               undef  dsD1if
#               define dsD1(msg) dsD_(msg,1)
#               define dsD1if(cond,msg) if (cond) { dsD1(msg); }
#       endif
#       if 0 == DS_DEBUG_LEVEL
#               undef  dsD0
#               undef  dsD0if
#               define dsD0(msg) dsD_(msg,0)
#               define dsD0if(cond,msg) if (cond) { dsD0(msg); }
#       endif
#       define dsD(msg) dsD3(msg)
#       define dsDif(cond,msg) if (cond) { dsD(msg); }
#  else
#       define dsD(msg) ((void)false)
#       define dsD0(msg) ((void)false)
#       define dsD1(msg) ((void)false)
#       define dsD2(msg) ((void)false)
#       define dsD3(msg) ((void)false)
#       define dsD4(msg) ((void)false)
#       define dsD5(msg) ((void)false)
#       define dsDif(cond,msg) ((void)false)
#       define dsD0if(cond,msg) ((void)false)
#       define dsD1if(cond,msg) ((void)false)
#       define dsD2if(cond,msg) ((void)false)
#       define dsD3if(cond,msg) ((void)false)
#       define dsD4if(cond,msg) ((void)false)
#       define dsD5if(cond,msg) ((void)false)
#  endif
//______________________________________________________________________________


////////////////////////////////////////////////////////////////////////////////
//      Logger
//  There are six logging levels, higher level means more log messages:
//  * dsL0
//  * dsL1
//  * dsL2
//  * dsL3
//  * dsL4
//  * dsL5
//  The default logger is set to level 3.
////////////////////////////////////////////////////////////////////////////////
#  if ENABLE_DS_LOG
#       define dsL_(msg,l)                                          \
  do {                                                              \
    ds_internal__debug::lock_t lk( ds_internal__debug::lock() );    \
    std::clog                                                       \
      <<__FILE__<<":"                                               \
      <<__LINE__<<":"                                               \
      <<"info("#l "): "<<msg                                        \
      <<", in "<<__FUNCTION__                                       \
      <<std::endl;                                                  \
  } while(0)
#       ifndef DS_LOG_LEVEL
#               define DS_LOG_LEVEL 0
#       endif
#       define dsL0(msg) ((void)false)
#       define dsL1(msg) ((void)false)
#       define dsL2(msg) ((void)false)
#       define dsL3(msg) ((void)false)
#       define dsL4(msg) ((void)false)
#       define dsL5(msg) ((void)false)
#       define dsL0if(cond,msg) ((void)false)
#       define dsL1if(cond,msg) ((void)false)
#       define dsL2if(cond,msg) ((void)false)
#       define dsL3if(cond,msg) ((void)false)
#       define dsL4if(cond,msg) ((void)false)
#       define dsL5if(cond,msg) ((void)false)
#       if 5 <= DS_LOG_LEVEL
#               undef  dsL5
#               undef  dsL5if
#               define dsL5(msg) dsL_(msg,5)
#               define dsL5if(cond,msg) if (cond) { dsL5(msg); }
#       endif
#       if 4 <= DS_LOG_LEVEL
#               undef  dsL4
#               undef  dsL4if
#               define dsL4(msg) dsL_(msg,4)
#               define dsL4if(cond,msg) if (cond) { dsL4(msg); }
#       endif
#       if 3 <= DS_LOG_LEVEL
#               undef  dsL3
#               undef  dsL3if
#               define dsL3(msg) dsL_(msg,3)
#               define dsL3if(cond,msg) if (cond) { dsL3(msg); }
#       endif
#       if 2 <= DS_LOG_LEVEL
#               undef  dsL2
#               undef  dsL2if
#               define dsL2(msg) dsL_(msg,2)
#               define dsL2if(cond,msg) if (cond) { dsL2(msg); }
#       endif
#       if 1 <= DS_LOG_LEVEL
#               undef  dsL1
#               undef  dsL1if
#               define dsL1(msg) dsL_(msg,1)
#               define dsL1if(cond,msg) if (cond) { dsL1(msg); }
#       endif
#       if 0 == DS_LOG_LEVEL
#               undef  dsL0
#               undef  dsL0if
#               define dsL0(msg) dsL_(msg,0)
#               define dsL0if(cond,msg) if (cond) { dsL0(msg); }
#       endif
#       define dsL(msg) dsL3(msg)
#       define dsLif(cond,msg) if (cond) { dsL(msg); }
#  else
#       define dsL(msg) ((void)false)
#       define dsL0(msg) ((void)false)
#       define dsL1(msg) ((void)false)
#       define dsL2(msg) ((void)false)
#       define dsL3(msg) ((void)false)
#       define dsL4(msg) ((void)false)
#       define dsL5(msg) ((void)false)
#       define dsLif(cond,msg) ((void)false)
#       define dsL0if(cond,msg) ((void)false)
#       define dsL1if(cond,msg) ((void)false)
#       define dsL2if(cond,msg) ((void)false)
#       define dsL3if(cond,msg) ((void)false)
#       define dsL4if(cond,msg) ((void)false)
#       define dsL5if(cond,msg) ((void)false)
#  endif
//______________________________________________________________________________

////////////////////////////////////////////////////////////////////////////////
//      Asserter(unimplemented)
// Usage:
//      dsI( p != 0 );
//      dsI( b == true );
////////////////////////////////////////////////////////////////////////////////
#  if ENABLE_DS_ASSERT
#       define dsI_(expr,msg)                                       \
  if ( !(expr) ) {                                                  \
    ds_internal__debug::lock_t lk( ds_internal__debug::lock() );    \
    std::clog                                                       \
      <<__FILE__<<":"                                               \
      <<__LINE__<<":"                                               \
      <<"debug(assertion): [FAILED] "#expr                          \
      <<std::endl                                                   \
      <<"\t[DESCRIPTION]: "<<msg                                    \
      <<std::endl;                                                  \
  } ( BOOST_ASSERT(expr) )
#       define dsI(expr) dsI_(expr, "(unspecified)")
#       define dsIm(expr,msg) dsI_(expr,msg)
#       define dsIif(cond,expr) if ( cond ) { dsI(expr); }
#       define dsImif(cond,expr,msg) if ( cond ) { dsIm(expr,msg); }
#  else
#       define dsI(expr)        ((void)false)
#       define dsIm(expr,msg)   ((void)false)
#       define dsIif(cond,expr) ((void)false)
#       define dsImif(cond,expr,msg) ((void)false)
#  endif
//______________________________________________________________________________

////////////////////////////////////////////////////////////////////////////////
//      TODO marker
// Usage:
//      TODO( msg );
////////////////////////////////////////////////////////////////////////////////
#  if defined(SHOW_TODO)
#       define TODO(msg)                                            \
  do {                                                              \
    ds_internal__debug::lock_t lk( ds_internal__debug::lock() );	\
    std::clog                                                       \
      <<__FILE__<<":"                                               \
      <<__LINE__<<":"                                               \
      <<"info(TODO): "<<msg                                         \
      <<", in "<<__FUNCTION__                                       \
      <<std::endl;                                                  \
  } while( 0 )
#  else
#       define TODO(msg)        ((void)false);
#  endif//SHOW_TODO
//______________________________________________________________________________

#endif//__DEBUG_HPP____by_Duzy_Chan__

