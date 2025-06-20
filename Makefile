CC ?= cc
NM ?= nm
STRIP ?= strip

ARCH ?= $(shell uname -m)
V := 0
O := .

DEBUG ?=

###############################################################################

arches := i386 x86_64

cflags := -Wall -Wextra -fPIC -fPIC -ffreestanding -nostartfiles -nostdlib -nodefaultlibs
cflags-i386 := -m32
cflags-x86_64 := -m64

ifneq ($(DEBUG),)
cflags += -D__EVILCC_DEBUG
endif

build-targets := $(addprefix build-,$(arches))
clean-targets := $(addprefix clean-,$(arches))

ifeq ($(V),0)
Q := @
else
Q :=
endif

override CFLAGS += $(cflags) $(cflags-$(ARCH))

###############################################################################

all: build

###############################################################################

.PHONY:
build: $(O)/evilcc_$(ARCH).o

.PHONY:
build-all: $(build-targets)

.PHONY:
$(build-targets):
	$(Q)$(MAKE) ARCH=${@:build-%=%} build

###############################################################################

.PHONY:
clean:
	@printf "  %-6s %s\n" "CLEAN" "$(ARCH)"
	$(Q)rm -f $(O)/evilcc.a

.PHONY:
clean-all: $(clean-targets)

.PHONY:
$(clean-targets):
	$(Q)$(MAKE) ARCH=${@:clean-%=%} clean

###############################################################################

.PHONY:
test: test/main-$(ARCH)

test/main-%:
	$(Q)$(MAKE) -C test/ ARCH="$*"

###############################################################################

$(O)/evilcc_%.o: src/init.c
	@mkdir -p $(O)
	@printf "  %-6s %s\n" "CC" "$@"
	$(Q)$(CC) $(CFLAGS) -Isrc -c $^ -o $@

ifeq ($(DEBUG),)
	@printf "  %-6s %s\n" "STRIP" "$@"
	$(Q)$(NM) $@ \
		| grep evilcc \
		| grep -v __evilcc_entry \
		| awk '{print "--strip-symbol=" $$NF}' \
		| xargs $(STRIP) $@
endif
