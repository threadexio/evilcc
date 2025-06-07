ARCH := $(shell uname -m)

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

###############################################################################

all: arch/$(ARCH)/evilcc.a

###############################################################################

# Clean only the entire build tree.
.PHONY:
clean:
	$(Q)rm -f $(target) $(objs) $(objs-$(ARCH))

###############################################################################

arch/$(ARCH)/evilcc.a: $(objs) $(objs-$(ARCH))
	@printf "  %-6s %s\n" "AR" "$@"
	$(Q)$(AR) r $@ $^

.PRECIOUS: arch/$(ARCH)/%.o
arch/$(ARCH)/%.o: arch/$(ARCH)/%.c
	@printf "  %-6s %s\n" "CC" "$@"
	$(Q)$(CC) $(CFLAGS) -c $^ -o $@

.PRECIOUS: %.o
%.o: %.c
	@printf "  %-6s %s\n" "CC" "$@"
	$(Q)$(CC) $(CFLAGS) -c $^ -o $@
