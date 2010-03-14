# -*- mode: Makefile:gnu -*-

V ?= debug
ifneq ($V,debug)
  V = release
endif

U := $(shell uname)
UNAME = $U

ifeq ($V,debug)
  COMMON_CXXFLAGS := -g -ggdb -std=gnu++0x
else
  COMMON_CXXFLAGS := -O2 -std=gnu++0x
endif

THIRD_DIR = $(TOP)/../third
BOOST_DIR = $(TOP)/../../open/boost_libraries
BOOST_LIB_DIR = $(BOOST_DIR)/stage-$V/lib
ifeq ($(UNAME),Linux)
  BOOST_LIB = boost_$1
else
  EXE = .exe
  ifeq ($V,debug)
    BOOST_LIB_SUFFIX = -mgw44-mt-sd
  else
    BOOST_LIB_SUFFIX = -mgw44-mt-s
  endif
  BOOST_LIB = boost_$1$(BOOST_LIB_SUFFIX)
endif

MKDIR = mkdir 

OUTOBJS = $(TOP)/out/$V/objs
OUTDEPS = $(TOP)/out/$V/deps
OUTLIB = $(TOP)/out/$V/lib
OUTBIN = $(TOP)/out/$V/bin

## Usage: $(call prepare_dir,dir)
prepare_dir = @[ -d $1 ] || $(MKDIR) -p $1
