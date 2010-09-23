#

sm.this.dir := $(call sm-this-dir)
sm.this.dirs :=

# TODO: fix conflicts with sm-load-subdirs in ../smart.mk
$(call sm-load-subdirs)

#$(call sm-load-module,$(sm.this.dir)/dsrc/smart.mk)
