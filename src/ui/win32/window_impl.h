/**
 *    Copyright 2010-09-11 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ext/slist>
#include <cstring>
#include <ds/graphics/box.hpp>
#include <ds/graphics/image.hpp>
#include <windows.h>

namespace ds { namespace ui {

    struct window::IMPL
    {
      screen::weak_ref _screen;

      HWND _hwnd;
      HDC _hdc;

      ds::graphics::image _image;

      __gnu_cxx::slist<ds::graphics::box> _dirtyRects;

      IMPL( const screen::pointer & d )
        : _screen( d )
        , _hwnd( NULL )
        , _hdc( NULL )
        , _image()
        , _dirtyRects()
      {
      }

      ~IMPL();

      void create( const window::pointer & );
      void destroy();

      void convert_pixels( int x, int y, int w, int h );

      void select_input(long mask);

      ds::graphics::box get_rect() const;

      ds::graphics::image * get_image_for_render();
      bool commit_image( const ds::graphics::box & dr );
    };//struct window::IMPL
    
  }//namespace ui
}//namespace ds
