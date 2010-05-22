# -*- mode: Makefile -*-
#

$(call sm-check-not-empty, ds.third.dir.tmp)
$(call sm-check-not-empty, ds.third.libxml)
#$(call sm-check-not-empty, ds.third.libxml.dir.tmp)
$(call sm-check-not-empty, ds.third.libpng)
$(call sm-check-not-empty, ds.third.libiconv)
$(call sm-check-not-empty, ds.third.zlib)

$(call sm-check-exists, $(ds.third.dir)/code/$(ds.third.libxml)/include)
#$(call sm-check-exists, $(ds.third.dir)/code/$(ds.third.libxml)/include/libxml/xmlversion.h)
#$(call sm-check-exists, $(ds.third.libxml.dir.tmp)/include/libxml/xmlversion.h)
$(call sm-check-exists, $(ds.third.dir.tmp)/$(ds.third.libxml)/include/libxml/xmlversion.h)
$(call sm-check-exists, $(ds.third.dir)/code/$(ds.third.libpng))
$(call sm-check-exists, $(ds.third.dir)/code/$(ds.third.libiconv))
$(call sm-check-exists, $(ds.third.dir)/code/$(ds.third.zlib))
