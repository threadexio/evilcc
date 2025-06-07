CFLAGS ?=
LFLAGS ?=

override CFLAGS := -Wall -Wextra $(CFLAGS)
override LFLAGS := -Wl,-e"__evilcc_entry" $(LFLAGS)

all: test/main

test/%: test/%.o src/evilcc.o
	$(CC) -o $@ $^ $(LFLAGS)
	sudo chown -v root:root $@
	sudo chmod -v +s $@
