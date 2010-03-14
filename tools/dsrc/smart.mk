# -*- mode: Makefile -*-
#

$(call sm-new-module, dsrc, executable)

include $(SM_MODULE_DIR)/check-deps.mk

SM_COMPILE_LOG := dsrc.log

SM_MODULE_INCLUDES := \
  -I$(DSUI_DIR)/include \
  -I$(THIRD_DIR)/code/libxml2-2.7.3/include \
  -I$(THIRD_DIR)/code/libiconv-1.13/include \
  -I$(THIRD_DIR)/code/libpng-1.2.35 \
  -I$(THIRD_DIR)/code/zlib-1.2.3 \
  -I$(BOOST_DIR)

SM_MODULE_COMPILE_FLAGS := \
  -DDS_TRACE_LEVEL=3 \
  -DDS_DEBUG_LEVEL=3 \
  -DDS_LOG_LEVEL=3 \

# SM_MODULE_LIB_PATH := \
#   -L$(THIRD_DIR)/libxml2-2.7.3/.libs \
#   -L$(THIRD_DIR)/libpng-1.2.35 \
#   -L$(THIRD_DIR)/zlib-1.2.3 \
#   -L$(BOOST_LIB_DIR)

SM_MODULE_LIB_PATH := \
  -L$(THIRD_DIR)/out/$V/lib \
  -L$(BOOST_LIB_DIR)

SM_MODULE_LIBS += \
  -l$(call BOOST_LIB, program_options) \
  -l$(call BOOST_LIB, filesystem) \
  -l$(call BOOST_LIB, system) \
  -lxml2 -lpng -lz

libxml2_link_to_winsock = yes
ifeq ($(libxml2_link_to_winsock),yes)
#  LDLIBS += -lnetapi32 -lrpcrt4 -lsnmpapi -liphlpapi \
#    -luser32 -lwsock32 -lwin32k -lws2_32 -ldnsapi -lshell32
  SM_MODULE_LIBS += -lwsock32
endif

SM_MODULE_SOURCES := dsrc.cpp

$(call sm-build-this)
