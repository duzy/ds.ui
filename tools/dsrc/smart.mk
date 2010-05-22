# -*- mode: Makefile -*-
#

$(call sm-new-module, dsrc, executable)

include $(sm.module.dir)/check-deps.mk

#SM_COMPILE_LOG := dsrc.log

sm.module.includes := \
  -I$(ds.ui.dir)/include \
  -I$(ds.third.dir.tmp)/$(ds.third.libxml)/include \
  -I$(ds.third.dir)/code/libxml2-2.7.3/include \
  -I$(ds.third.dir)/code/libiconv-1.13/include \
  -I$(ds.third.dir)/code/libpng-1.2.35 \
  -I$(ds.third.dir)/code/zlib-1.2.3 \
  -I$(ds.third.boost.dir)

sm.module.options.compile := \
  -DDS_TRACE_LEVEL=3 \
  -DDS_DEBUG_LEVEL=3 \
  -DDS_LOG_LEVEL=3 \

# sm.module.dirs.lib := \
#   -L$(ds.third.dir)/libxml2-2.7.3/.libs \
#   -L$(ds.third.dir)/libpng-1.2.35 \
#   -L$(ds.third.dir)/zlib-1.2.3 \
#   -L$(ds.third.boost.dir.lib)

sm.module.dirs.lib := \
  -L$(ds.third.dir)/out/$V/lib \
  -L$(ds.third.boost.dir.lib)

sm.module.libs += \
  -l$(call ds.third.boost.use, program_options) \
  -l$(call ds.third.boost.use, filesystem) \
  -l$(call ds.third.boost.use, system) \
  -lxml2 -lpng -lz

libxml2_link_to_winsock = yes
ifeq ($(libxml2_link_to_winsock),yes)
#  LDLIBS += -lnetapi32 -lrpcrt4 -lsnmpapi -liphlpapi \
#    -luser32 -lwsock32 -lwin32k -lws2_32 -ldnsapi -lshell32
  sm.module.libs += -lwsock32
endif

sm.module.sources := dsrc.cpp

$(call sm-build-this)
