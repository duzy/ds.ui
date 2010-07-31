#

$(call sm-new-module, all-units, tests)
$(call sm-check-not-empty, ds.ui.dir)
$(call sm-check-not-empty, ds.ui.dir.lib)

sm.module.lang := c++

sm.module.includes := \
  $(ds.ui.dir)/include \
  $(ds.third.dir.inc) \
  $(ds.third.dir.inc)/zlib \
  $(ds.third.dir.inc)/libpng \
  $(ds.third.boost.dir)

sm.module.sources := \
  boost_test_stuff.cpp \
  shared_object.t \
  streambuf.t \
  png_reader.t \

sm.module.sources.generated := \
  $(sm.dir.out.tmp)/dsrc_test_stub.cpp

$(sm.dir.out.tmp)/dsrc_test_stub.cpp: \
  $(sm.module.dir)/dsrc_test.xml \
  $(sm.dir.out.bin)/dsrc
	$(sm.var.Q) $(sm.dir.out.bin)/dsrc -o $@ $<

sm.module.libdirs := \
  $(ds.ui.dir.lib) \
  $(ds.third.boost.dir.lib)

sm.module.rpath := \
  $(ds.ui.dir.lib) \
  $(ds.third.dir.lib) \

sm.module.libs := dsui \
  $(call ds.third.boost.use, system) \
  $(call ds.third.boost.use, filesystem) \

$(sm-build-this)

