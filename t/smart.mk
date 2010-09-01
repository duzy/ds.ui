#

$(call sm-new-module, unit-tests, tests)
$(call sm-check-not-empty, ds.ui.dir)
$(call sm-check-not-empty, ds.ui.dir.lib)

sm.module.lang := c++

sm.module.includes := \
  $(ds.ui.dir)/include \
  $(ds.third.dir.inc) \
  $(ds.third.boost.dir)

sm.module.sources := \
  boost_test_stuff.cpp \
  shared_object.t

sm.module.libdirs := \
  -L$(ds.ui.dir.lib) \
  -L$(ds.third.boost.dir.lib)

sm.module.rpath := \
  $(ds.ui.dir.lib) \

sm.module.libs := dsui \
  $(call ds.third.boost.use, system) \
  $(call ds.third.boost.use, filesystem) \

$(sm-build-this)

