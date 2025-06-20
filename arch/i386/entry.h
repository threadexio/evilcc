#ifndef __EVILCC_ARCH_I386_ENTRY_H
#define __EVILCC_ARCH_I386_ENTRY_H

#include "compiler.h"
#include "config.h"

// IMPORTANT: BE VERY CAREFUL IN THIS FILE!
//
// Everything here is supported by thin strings of hope. All pointers wired up
// correctly (I hope) and all offsets in perfect balance.
//
// Before you make any changes to this file, make sure you absoluterly know what
// every single lines does.

used static void __evilcc_init(int argc, const char* argv[], const char* envp[]);

always_inline noreturn static void evilcc_finish(void) {
  asm volatile(
    // The `+0x2a` is the exact offset needed to jump to the instruction below
    // `jmp __evilcc_init` in `__evilcc_entry`. If this number and the code of
    // `__evilcc_entry` do not match... prepare for unforeseen consequences.
    "jmp " stringify(__EVILCC_ENTRY_SYMBOL) "+0x2a"
    :::
  );

  unreachable();
}

asm (
  ".data\n"
  ".local __evilcc_stack\n"
  "__evilcc_stack: .quad 0\n"

  ".text\n"
  ".globl " stringify(__EVILCC_ENTRY_SYMBOL) "\n"
  stringify(__EVILCC_ENTRY_SYMBOL) ":\n"
  "pusha\n"
  "pushfl\n"

  // Save the stack pointer for later. The state of the envrionment after
  // execution returns to `__evilcc_entry` is all messed up (because
  // `__evilcc_init` did not do any cleanup). We save the stack pointer to
  // global variable so we can safely restore it and then the saved context of
  // the CPU.
  "lea __evilcc_stack, %ebx\n"
  "mov %esp, (%ebx)\n"

  // Load the address of the stack before we saved our context.
  "lea 36(%esp), %ebx\n"

  // Prepare arguments.
  "sub $12, %esp\n"
  "mov 0(%ebx), %eax\n"
  "mov %eax, 0(%esp)\n"          // argc
  "lea 8(%ebx, %eax, 4), %eax\n"
  "mov %eax, 8(%esp)\n"          // envp
  "lea 4(%ebx), %eax\n"
  "mov %eax, 4(%esp)\n"          // argv

  // Add an extra 4 bytes as a dummy return address. This is needed because we
  // are emulating a `call` instruction with a plain `jmp`. `__evilcc_init`
  // expects to be given execution from a `call`, so we have to emulate adding
  // a return address to the stack. We need to do this to avoid having a `ret`
  // instruction at the end of `__evilcc_init`.
  "sub $4, %esp\n"
  "jmp __evilcc_init\n"

  // Load the saved stack pointer.
  "lea __evilcc_stack, %ebx\n"
  "mov (%ebx), %esp\n"

  "popfl\n"
  "popa\n"

  "jmp " stringify(__EVILCC_REAL_ENTRY_SYMBOL) "\n"
);

#endif
