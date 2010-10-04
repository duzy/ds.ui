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
  streambuf.t \

#sm.this.sources.external := \
#  $(sm.out.tmp)/dsrc_test_stub.cc

EXE := $(if $(sm.os.name.win32),.exe)

$(sm.out.tmp)/dsrc_test_stub.cc: \
  $(sm.this.dir)/dsrc_test.xml | $(sm.out.bin)/dsrc$(EXE)
	@$(call sm-util-mkdir,$(@D))
	@$(sm.out.bin)/dsrc -o $@ $<

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

