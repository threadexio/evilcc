#ifndef __EVILCC_ARCH_I386_ENTRY_H
#define __EVILCC_ARCH_I386_ENTRY_H

#include "compiler.h"
#include "config.h"

used static void __evilcc_init(int argc, const char* argv[], const char* envp[]);

always_inline noreturn static void evilcc_finish(void) {
  asm volatile(
    "call __evilcc_get_pc\n"
    "add $(__evilcc_init_ret - .), %%eax\n"
    "jmp *%%eax\n"
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

  ".local __evilcc_init_ret\n"
  "__evilcc_init_ret:\n"

  // Load the saved stack pointer.
  "lea __evilcc_stack, %ebx\n"
  "mov (%ebx), %esp\n"

  "popfl\n"
  "popa\n"

  "jmp " stringify(__EVILCC_REAL_ENTRY_SYMBOL) "\n"

  // Small function to load `eip` into `eax`. There is no direct way to access
  // `eip` so we must get it from the return address saved by `call`.
  ".local __evilcc_get_pc\n"
  "__evilcc_get_pc:\n"
  "mov (%esp), %eax\n"
  "ret\n"
);

#endif
