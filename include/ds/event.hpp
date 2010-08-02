/**
 *    Copyright 2010-07-18 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#ifndef __DS_EVENT_HPP____by_Duzy_Chan__
#define __DS_EVENT_HPP____by_Duzy_Chan__ 1

namespace ds
{
  struct event
  {
    int type;

    template<class SubEvent, int TV>
    struct sub;

    struct quit;
    struct custom;

  protected:
    event(int v) : type(v) {}
  };//struct event

  template<class SubEvent, int TV>
  struct event::sub : event
  {
    enum { TypeValue = TV };
    sub() : event( TypeValue ) {}
  };

  struct event::quit : event::sub<quit, 1> {};
  struct event::custom : event::sub<custom, 2> {};
}//namespace ds

#endif//__DS_EVENT_HPP____by_Duzy_Chan__
