ARCH ?= $(shell uname -m)
V := 0
O := .

###############################################################################


arches := $(patsubst arch/%,%,$(wildcard arch/*))
build-targets := $(addprefix build-,$(arches))
clean-targets := $(addprefix clean-,$(arches))

build-configs := $(shell find . -type f -name build.mk -printf '%P\n')

final-cflags := $(CFLAGS)
final-objs :=

define source-build-config =
cflags :=
objs :=
cflags-$$(ARCH) :=
objs-$$(ARCH) :=

include $(1)
final-cflags += $$(cflags) $$(cflags-$$(ARCH))
final-objs += $$(addprefix $$(O)/$$(dir $(1)), $$(objs) $$(objs-$$(ARCH)) )
endef

$(foreach x,$(build-configs),$(eval $(call source-build-config,$(x))))

override CFLAGS := $(final-cflags)
override OBJS := $(final-objs)
undefine final-cflags final-objs

-include .config.mk

###############################################################################

ifeq ($(V),0)
Q := @
else
Q :=
endif

all: build

.PHONY:
build: $(O)/arch/$(ARCH)/evilcc.a

.PHONY:
$(build-targets):
	$(Q)$(MAKE) ARCH=${@:build-%=%} build

.PHONY:
clean:
	@printf "  %-6s %s\n" "CLEAN" "$(ARCH)"
	$(Q)rm -f $(O)/arch/$(ARCH)/evilcc.a $(final-objs)

.PHONY:
clean-all: $(clean-targets)

.PHONY:
$(clean-targets):
	$(Q)$(MAKE) ARCH=${@:clean-%=%} clean

###############################################################################

.PHONY:
test:
	$(Q)$(MAKE) -C test/ ARCH="$(ARCH)"

###############################################################################

$(O)/arch/$(ARCH)/evilcc.a: $(final-objs)
	@printf "  %-6s %s\n" "AR" "${@:$(O)/%=%}"
	$(Q)$(AR) r $@ $^

.PRECIOUS: %.o
$(O)/%.o: %.c
	@printf "  %-6s %s\n" "CC" "${@:$(O)/%=%}"
	@mkdir -p $(dir $@)
	$(Q)$(CC) $(CFLAGS) -c $^ -o $@

