ARCH := $(shell uname -m)
O := $(shell pwd)/out


all: main-$(ARCH) libevil-$(ARCH).so

main-$(ARCH): main.c $(O)/evilcc_$(ARCH).o
	$(CC) $(CFLAGS) $^ -o $@ -Wl,-e__evilcc_entry
	sudo chown -v root:root $@
	sudo chmod -v u+rws,g+rs,o+rx $@

libevil-$(ARCH).so: evil.c
	$(CC) $(CFLAGS) -shared -fPIC $^ -o $@

$(O)/evilcc_$(ARCH).o:
	$(MAKE) -C ../ O="$(O)" ARCH="$(ARCH)" build
