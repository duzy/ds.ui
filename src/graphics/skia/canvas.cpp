/**
 *    Copyright 2010-08 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/graphics/canvas.hpp>
#include <ds/graphics/gil/image.hpp>
#include <ds/graphics/image.hpp>
#include <ds/graphics/box.hpp>
#include <ds/graphics/segment.hpp>
#include <ds/graphics/polygon.hpp>
#include <ds/graphics/color.hpp>
#include <ds/graphics/drawing_tools.hpp>
#include <skia/core/SkCanvas.h>
#include <skia/core/SkBitmap.h>
#include <skia/core/SkRect.h>
#include <skia/core/SkPath.h>
#include <ds/debug.hpp>

namespace ds { namespace graphics {

    static void to_SkPaint( SkPaint & paint, const drawing_tool & d )
    {
      // TODO: ...
    }

    static void to_SkPaint( SkPaint & paint, const brush & b )
    {
      // TODO: ...
    }

    static void to_SkPaint( SkPaint & paint, const pen & p )
    {
      // TODO: ...
    }

    //--------------------------------------------------------------------
    
    struct canvas::IMPL
    {
      image * _image;

      SkBitmap _skBitmap;
      SkCanvas _skCanvas;

      IMPL( image & img )
        : _image( &img )
      {
        SkBitmap::Config config = SkBitmap::kNo_Config;
        switch ( _image->pixel_type() ) {
        case image::ARGB_8888_PIXEL:
          config = SkBitmap::kARGB_8888_Config;
          break;
        case image::ARGB_4444_PIXEL:
          config = SkBitmap::kARGB_4444_Config;
          break;
        case image::RGB_565_PIXEL:
          config = SkBitmap::kRGB_565_Config;
          break;
        default:
          dsIm( false, "unsupported pixel format" );
          break;
        }

        _skBitmap.setConfig(config, _image->width(), _image->height());
        _skBitmap.setPixels(_image->pixels());
        _skCanvas.setBitmapDevice(_skBitmap);
      }

      template<typename G, typename D>
      inline void draw( const G & g, const D & d, SkPaint::Style style )
      {
        SkPaint paint;                  to_SkPaint( paint, d );
        paint.setStyle( style );
        this->draw( g, paint );
      }

      void draw( const point & g,       const SkPaint & p );
      void draw( const segment & g,     const SkPaint & p );
      void draw( const box & g,         const SkPaint & p );
      void draw( const polygon & g,     const SkPaint & p );
    };//struct canvas::IMPL

    void canvas::IMPL::draw( const point & g, const SkPaint & p )
    {
      _skCanvas.drawPoint(g.x(), g.y(), p);
    }

    void canvas::IMPL::draw( const segment & g, const SkPaint & p )
    {
      _skCanvas.drawLine(g.first .x(), g.first .y(),
                         g.second.x(), g.second.y(),
                         p);
    }

    void canvas::IMPL::draw( const box & g, const SkPaint & p )
    {
      _skCanvas.drawRect(SkRect::MakeLTRB(g.min_corner().x(),
                                          g.min_corner().y(),
                                          g.max_corner().x(),
                                          g.max_corner().y()),
                         p);
    }

    void canvas::IMPL::draw( const polygon & g, const SkPaint & skPaint )
    {
      if ( g.outer().empty() )
        return;

      SkPath skPath;
      {
        polygon::ring_type::const_iterator it( g.outer().begin() );
        skPath.moveTo(SkPoint::Make(it->x(), it->y()));
        for(++it; it != g.outer().end(); ++it)
          skPath.lineTo(SkPoint::Make(it->x(), it->y()));
        _skCanvas.drawPath(skPath, skPaint);
      }

      if (g.inners().empty())
        return;

      {
        polygon::inner_container_type::const_iterator inner;
        for( inner  = g.inners().begin();
             inner != g.inners().end(); ++inner ) {
          if ( inner->empty() ) continue;

          polygon::ring_type::const_iterator it( inner->begin() );

          skPath.rewind();
          skPath.moveTo(SkPoint::Make(it->x(), it->y()));
          for(; it != inner->end(); ++it )
            skPath.lineTo(SkPoint::Make(it->x(), it->y()));

          _skCanvas.drawPath(skPath, skPaint);
        }//foreach inner
      }
    }

    //////////////////////////////////////////////////////////////////////

    canvas::canvas( image & img )
      : _imp( new IMPL(img) )
    {
    }

    canvas::~canvas()
    {
      delete _imp;
    }

    bool canvas::clip( const box & g )
    {
      return _imp->_skCanvas.clipRect(SkRect::MakeLTRB(g.min_corner().x(),
                                                       g.min_corner().y(),
                                                       g.max_corner().x(),
                                                       g.max_corner().y()),
                                      SkRegion::kReplace_Op);
    }
    
    bool canvas::clip( const ring & g )
    {
      // TODO: ...
      return false;
    }

    bool canvas::clip( const polygon & g )
    {
      SkPath skPath;
      if ( !g.outer().empty() ) {
        polygon::ring_type::const_iterator it( g.outer().begin() );
        skPath.moveTo(SkPoint::Make(it->x(), it->y()));
        for(++it; it != g.outer().end(); ++it)
          skPath.lineTo(SkPoint::Make(it->x(), it->y()));
      }
      return _imp->_skCanvas.clipPath(skPath, SkRegion::kReplace_Op);
    }

    brush & canvas::default_brush()
    {
      static brush sDefaultBrush;
      return sDefaultBrush;
    }

    pen & canvas::default_pen()
    {
      static pen sDefaultPen;
      return sDefaultPen;
    }

    void canvas::render( const point & g, const brush & p )
    {
      _imp->draw( g, p, SkPaint::kFill_Style );
    }

    void canvas::render( const box & g, const brush & p )
    {
      _imp->draw( g, p, SkPaint::kFill_Style );
    }

    void canvas::render( const polygon & g, const brush & p )
    {
      _imp->draw( g, p, SkPaint::kFill_Style );
    }//canvas::render

    void canvas::render( const color & c )
    {
      _imp->_skCanvas.drawARGB( c.alpha, c.red, c.green, c.blue );
    }

    void canvas::render( const image & img )
    {
      // TODO: ...
    }

    void canvas::stroke( const point & g, const pen & p )
    {
      _imp->draw( g, p, SkPaint::kStroke_Style );
    }

    void canvas::stroke( const segment & g , const pen & p )
    {
      _imp->draw( g, p, SkPaint::kStroke_Style );
    }

    void canvas::stroke( const polygon & g, const pen & p )
    {
      _imp->draw( g, p, SkPaint::kStroke_Style );
    }

    void canvas::stroke( const box & g, const pen & p )
    {
      _imp->draw( g, p, SkPaint::kStroke_Style );
    }

  }//namespace graphics
}//namespace ds
