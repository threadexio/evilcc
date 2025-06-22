# evilcc

A library-like alternate ELF entrypoint with super powers.

This is a static library that provides a shim entrypoint for the executable
which can be configured to:

1. Run with the modified real/effective/saved uids/gids.
2. Alter personality.

And this is done fully transparently to the rest of the code with minimal
footprint.

This is useful in CTF challenges that provide executables that need specific
mitigations, like ASLR, turned off completely even for dynamic libraries
without requiring the player to set it globally for the whole system via
`randomize_va_space`. Additionally, functionality 1 can be used with
setuid/setgid executables to fully promote the user to the owner of the
executable.

Special attention was given to make the footprint of this library as small as
possible. Because this is intended for CTF challenges, it would be less than
ideal if, say, the code of `evilcc` could be used as part of ROP chain to solve
the challenge in an uninteded way. For this reason, all system calls (and
functions) are inlined at each call site. This _does_ lead to increased binary
sizes since code must be duplicated. But since `evilcc` contains very little
code anyway (compared to the average executable), the size overhead is minimal.

Because `evilcc` needs handwritten assembly code to work correctly, support for
each architecture must be added manually. Although abstractions exist, deep
knowledge of each architecture is still needed. Currently the following
architectures are supported:

* [x] `i386`
* [x] `x86_64`

## How it works

This project compiles down to a static library that provides the
`__evilcc_entry` symbol. That symbol is supposed to be set as the executable's
entrypoint. At execution time, the loader (or kernel) will jump to
`__evilcc_entry` which will configure the environment as needed and transfer
execution to the "real" entrypoint of the executable (`_start`). After
`__evilcc_entry` finishes, no code of `evilcc` is ever called again.

## Testing it out

1. Clone the repo locally.

2. Build the test binary.

```bash
$ make -C test
$ ./test/build/main
```

The test binary prints some diagnostic information about the environment and
then exits. Depending on how `evilcc` is configured, the output will look
different.

`evilcc` is configured via the preprocessor. For example, to enable the
functionality of 1, `evilcc` must be built like this:

```bash
$ make -C test/ CFLAGS="--setuid -1"
$ ./test/build/main
```

Since the test binary is marked setuid and owned by `root` you should see the
real uid of the test binary is `0`, the same as the effective uid. Additionally,
the real gid is unchanged but the effective gid is `0`. This is because the test
binary is also marked as setgid but `evilcc` was not configured to promote the
effective gid to the real gid. Configuration flags and more docs are available
in [`src/config.h`](./src/config.h).

## Building with evilcc

The most basic way to use `evilcc` is like this:

```bash
$ ./evilcc main.c -o main
```

`evilcc` is a compiler wrapper script that automatically bundles in `evilcc`.
It is also possible to use `evilcc` without this wrapper script by doing the
following:

```bash
$ cc main.c -o main.o
$ cc main.o evilcc/evilcc_x86_64.o -o main -Wl,-e__evilcc_entry
```

The important parts are that you need to include the `evilcc` object containing
the `evilcc` code and to instruct the linker to set the entrypoint of the
executable to `__evilcc_entry`. The latter is done with the `-Wl,-e` argument.

With some work, it can be integrated with make.

```Makefile
EVILCC := ../evilcc
EVILCC_OUT := $(shell pwd)/build/evilcc
EVILCC_CFLAGS :=

main: main.o $(EVILCC_OUT)/evilcc_x86_64.o
	$(CC) $^ -o $@ -Wl,-e__evilcc_entry $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(EVILCC_OUT)/evilcc_%.o:
	$(MAKE) -C $(EVILCC)/ O="$(EVILCC_OUT)" ARCH="$*" CFLAGS="$(EVILCC_CFLAGS)" build
```

And now everything should just work with a single `make`.

A crude way to check whether `evilcc` is correctly linked into the executable is
with:

```bash
$ nm <executable> | grep __evilcc
```

If there are symbols named that, then it's probably correctly linked in. You can
also verify that the entrypoint is set correctly with `readelf`. A better way
to check would be to compile `evilcc` in debug mode and run the final executable
once. If `evilcc` is linked in correctly, then it should print some debugging
messages in the programs stderr.

The build of `evilcc` itself can be configured with the `CFLAGS` parameter
passed to `make`. For example, to compile with PIC you do: `make CFLAGS="-fPIC"`
in the repo root. If using the `Makefile` template above, you need to set
`EVILCC_CFLAGS` obviously.
