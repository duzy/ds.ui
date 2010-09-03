#

$(call sm-new-module, all-units, tests)
$(call sm-check-not-empty, ds.ui.dir)
$(call sm-check-not-empty, ds.ui.dir.lib)

sm.module.lang := c++

sm.module.includes := \
  $(ds.ui.dir)/include \
  $(ds.third.dir.inc) \
  $(ds.third.dir.inc)/libpng \
  $(ds.third.boost.dir)

sm.module.sources := \
  boost_test_stuff.cpp \
  shared_object.t \
  streambuf.t \
  png_reader.t \

sm.module.sources.generated := \
  $(sm.dir.out.tmp)/dsrc_test.cpp

$(sm.dir.out)/temp/dsrc_test.cpp: \
  $(sm.dir.out.bin)/dsrc \
  $(sm.module.dir)/t/dsrc_test.xml
	echo "TODO: $@"

sm.module.libdirs := \
  -L$(ds.ui.dir.lib) \
  -L$(ds.third.boost.dir.lib)

sm.module.rpath := \
  $(ds.ui.dir.lib) \
  $(ds.third.dir.lib) \

sm.module.libs := dsui \
  $(call ds.third.boost.use, system) \
  $(call ds.third.boost.use, filesystem) \

$(sm-build-this)

