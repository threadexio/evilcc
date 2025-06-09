ARCH ?= $(shell uname -m)
V := 0

ifeq ($(V),0)
Q := @
else
Q :=
endif

O := .

###############################################################################

# Load non-arch-specific objects, flags, etc.
include build.mk
objs := $(addprefix $(O)/,$(objs))

# Load arch-specific objects, flags, etc.
include arch/$(ARCH)/build.mk
objs-$(ARCH) := $(addprefix $(O)/arch/$(ARCH)/,$(objs-$(ARCH)))
override CFLAGS += $(cflags)

-include .config.mk

arches := $(patsubst arch/%,%,$(wildcard arch/*))

build-targets := $(addprefix build-,$(arches))
clean-targets := $(addprefix clean-,$(arches))

###############################################################################

all: build

.PHONY:
build: $(O)/arch/$(ARCH)/evilcc.a

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
	$(Q)rm -f $(O)/arch/$(ARCH)/evilcc.a $(objs) $(objs-$(ARCH))

.PHONY:
clean-all: $(clean-targets)

.PHONY:
$(clean-targets):
	$(Q)$(MAKE) ARCH=${@:clean-%=%} clean

###############################################################################

$(O)/arch/$(ARCH)/evilcc.a: $(objs) $(objs-$(ARCH))
	@printf "  %-6s %s\n" "AR" "$@"
	$(Q)$(AR) r $@ $^

.PRECIOUS: %.o
$(O)/%.o: %.c
	@printf "  %-6s %s\n" "CC" "$@"
	@mkdir -p $(dir $@)
	$(Q)$(CC) $(CFLAGS) -c $^ -o $@

