ARCH := $(shell uname -m)
O := $(shell pwd)/out


all: main-$(ARCH)

main-$(ARCH): main.c $(O)/arch/$(ARCH)/evilcc.a
	$(CC) $(CFLAGS) $^ -o $@ -Wl,-e__evilcc_entry
	sudo chown -v root:root $@
	sudo chmod -v +s $@

$(O)/arch/$(ARCH)/evilcc.a:
	$(MAKE) -C ../ O="$(O)" ARCH="$(ARCH)" build
