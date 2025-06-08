override LFLAGS += -Wl,-e"__evilcc_entry"

all: test/main-x86_64 test/main-i386

test/main-x86_64: test/main.c src/arch/x86_64/evilcc.a
	$(CC) $(CFLAGS) -g3 -m64 $^ -o $@ $(LFLAGS)
	sudo chown -v root:root $@
	sudo chmod -v +s $@

test/main-i386: test/main.c src/arch/i386/evilcc.a
	$(CC) $(CFLAGS) -g3 -m32 $^ -o $@ $(LFLAGS)
	sudo chown -v root:root $@
	sudo chmod -v +s $@

.PRECIOUS: src/arch/%/evilcc.a
src/arch/%/evilcc.a:
	$(MAKE) -C src/ ARCH=$* build
