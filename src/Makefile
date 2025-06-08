ARCH ?= $(shell uname -m)
V := 0

ifeq ($(V),0)
Q := @
else
Q :=
endif

###############################################################################

# Load non-arch-specific objects, flags, etc.
include build.mk

# Load arch-specific objects, flags, etc.
include arch/$(ARCH)/build.mk
objs-$(ARCH) := $(addprefix arch/$(ARCH)/,$(objs-$(ARCH)))
override CFLAGS += $(cflags)

-include .config.mk

arches := $(patsubst arch/%,%,$(wildcard arch/*))

build-targets := $(addprefix build-,$(arches))
clean-targets := $(addprefix clean-,$(arches))

###############################################################################

all: build

.PHONY:
build: arch/$(ARCH)/evilcc.a

.PHONY:
.NOTPARALLEL:
build-all: $(build-targets)

.PHONY:
$(build-targets):
	$(Q)$(MAKE) ARCH=${@:build-%=%} build

###############################################################################

.PHONY:
clean:
	@printf "  %-6s %s\n" "CLEAN" "$(ARCH)"
	$(Q)rm -f arch/$(ARCH)/evilcc.a $(objs) $(objs-$(ARCH))

.PHONY:
clean-all: $(clean-targets)

.PHONY:
$(clean-targets):
	$(Q)$(MAKE) ARCH=${@:clean-%=%} clean

###############################################################################

arch/$(ARCH)/evilcc.a: $(objs) $(objs-$(ARCH))
	@printf "  %-6s %s\n" "AR" "$@"
	$(Q)$(AR) r $@ $^

.PRECIOUS: %.o
%.o: %.c
	@printf "  %-6s %s\n" "CC" "$@"
	$(Q)$(CC) $(CFLAGS) -c $^ -o $@
