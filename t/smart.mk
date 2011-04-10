#

$(call sm-new-module, all-units, tests)
$(call sm-check-not-empty, ds.ui.dir)
$(call sm-check-not-empty, ds.ui.dir.lib)

sm.this.verbose := true
sm.this.toolset := gcc
sm.this.lang := c++

sm.this.includes := \
  $(ds.ui.dir)/include \
  $(ds.third.dir.inc) \
  $(ds.third.dir.inc)/zlib \
  $(ds.third.dir.inc)/libpng \
  $(ds.third.boost.dir) \
  $(ds.third.boost.geometry)

sm.this.sources := \
  boost_test_stuff.cpp \
  shared_object.t \

sm.this.link.options := \
  -Wl,--rpath,$(ds.ui.dir.lib) \
  -Wl,--rpath,$(ds.third.dir.lib)

sm.this.libdirs := \
  $(ds.ui.dir.lib) \
  $(ds.ge.dir.lib) \
  $(ds.third.dir.lib) \
  $(ds.third.boost.dir.lib)

sm.this.libs := dsui dsge \
  $(call ds.third.boost.use, system) \
  $(call ds.third.boost.use, filesystem) \
  $(ds.third.libpng.libname) \

$(sm-build-this)

