#
# module.mk
#
# Copyright 2017 Stéphane Lepin <stephane.lepin@gmail.com
#

MOD		:= v23
$(MOD)_SRCS	+= v23.c v23_in.c v23_out.c

include mk/mod.mk
