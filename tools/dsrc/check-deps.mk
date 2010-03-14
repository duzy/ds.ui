# -*- mode: Makefile -*-
#

## libxml2
n := libxml2-2.7.6
$(call check-exists, $(THIRD_DIR)/code/$n/include,\
  libxml2 is not ready)
# $(call check-exists, $(THIRD_DIR)/$n/.libs/libxml2.*,\
#   libxml2 is not ready)

## libpng
$(call check-exists, $(THIRD_DIR)/code/libpng-1.2.35,\
  libpng not found )

## zlib
$(call check-exists, $(THIRD_DIR)/code/zlib-1.2.3,\
  zlib not fount)

# ## libiconv
# $(call check-exists, $(THIRD_DIR)/code/libiconv-1.13/include/iconv.h,\
#   libiconv is not ready)
