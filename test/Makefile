export _CC := $(CC)
CC := ../evilcc

all: build/main build/libevil.so

build/main: main.c
	mkdir -p build
	$(CC) $(CFLAGS) $^ -o $@
	sudo chown -v root:root $@
	sudo chmod -v u+rws,g+rs,o+rx $@

build/libevil.so: evil.c
	mkdir -p build
	$(CC) $(CFLAGS) -shared -fPIC $^ -o $@
