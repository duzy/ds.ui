#

$(call sm-new-module, all-units, tests)
$(call sm-check-not-empty, ds.ui.dir)
$(call sm-check-not-empty, ds.ui.dir.lib)

sm.this.lang := c++

sm.this.includes := \
  $(ds.ui.dir)/include \
  $(ds.third.dir.inc) \
  $(ds.third.dir.inc)/zlib \
  $(ds.third.dir.inc)/libpng \
  $(ds.third.boost.dir)

sm.this.sources := \
  boost_test_stuff.cpp \
  shared_object.t \
  streambuf.t \
  png_reader.t \

sm.this.sources.generated := \
  $(sm.out.tmp)/dsrc_test_stub.cpp

$(sm.out.tmp)/dsrc_test_stub.cpp: \
  $(sm.this.dir)/dsrc_test.xml \
  $(sm.out.bin)/dsrc
	$(sm.var.Q) $(sm.out.bin)/dsrc -o $@ $<

sm.this.libdirs := \
  $(ds.ui.dir.lib) \
  $(ds.third.boost.dir.lib)

sm.this.rpath := \
  $(ds.ui.dir.lib) \
  $(ds.third.dir.lib) \

sm.this.libs := dsui \
  $(call ds.third.boost.use, system) \
  $(call ds.third.boost.use, filesystem) \

$(sm-build-this)

