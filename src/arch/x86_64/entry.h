#ifndef __EVILCC_ARCH_X86_64_ENTRY_H
#define __EVILCC_ARCH_X86_64_ENTRY_H

#include "compiler.h"
#include "config.h"

used static void __evilcc_init(int argc, const char* argv[], const char* envp[]);

always_inline noreturn static void evilcc_finish(void) {
  asm volatile(
    "lea __evilcc_init_ret(%%rip), %%rax\n"
    "jmp *%%rax\n"
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

  "movq %rax,   -8(%rsp)\n"
  "movq %rbx,  -16(%rsp)\n"
  "movq %rcx,  -24(%rsp)\n"
  "movq %rdx,  -32(%rsp)\n"
  "movq %rsi,  -40(%rsp)\n"
  "movq %rdi,  -48(%rsp)\n"
  "movq %rsp,  -56(%rsp)\n"
  "movq %rbp,  -64(%rsp)\n"
  "movq %r8,   -72(%rsp)\n"
  "movq %r9,   -80(%rsp)\n"
  "movq %r10,  -88(%rsp)\n"
  "movq %r11,  -96(%rsp)\n"
  "movq %r12, -104(%rsp)\n"
  "movq %r13, -112(%rsp)\n"
  "movq %r14, -120(%rsp)\n"
  "movq %r15, -128(%rsp)\n"
  "sub $128, %rsp\n"

  // Save the stack pointer for later. The state of the envrionment after
  // execution returns to `__evilcc_entry` is all messed up (because
  // `__evilcc_init` did not do any cleanup). We save the stack pointer to
  // global variable so we can safely restore it and then the saved context of
  // the CPU.
  "lea __evilcc_stack(%rip), %rbx\n"
  "mov %rsp, (%rbx)\n"

  // Load the address of the stack before we saved our context.
  "lea 128(%rsp), %rbx\n"

  // Prepare arguments.
  "mov 0(%rbx), %rdi\n"           // argc
  "lea 8(%rbx), %rsi\n"           // argv
  "lea 16(%rbx, %rdi, 8), %rdx\n" // envp

  // Add an extra 8 bytes as a dummy return address. This is needed because we
  // are emulating a `call` instruction with a plain `jmp`. `__evilcc_init`
  // expects to be given execution from a `call`, so we have to emulate adding
  // a return address to the stack. We need to do this to avoid having a `ret`
  // instruction at the end of `__evilcc_init`.
  "sub $8, %rsp\n"
  "jmp __evilcc_init\n"

  ".local __evilcc_init_ret\n"
  "__evilcc_init_ret:\n"

  // Load the saved stack pointer.
  "lea __evilcc_stack(%rip), %rbx\n"
  "mov (%rbx), %rsp\n"

  "add $128, %rsp\n"
  "movq   -8(%rsp), %rax\n"
  "movq  -16(%rsp), %rbx\n"
  "movq  -24(%rsp), %rcx\n"
  "movq  -32(%rsp), %rdx\n"
  "movq  -40(%rsp), %rsi\n"
  "movq  -48(%rsp), %rdi\n"
  "movq  -56(%rsp), %rsp\n"
  "movq  -64(%rsp), %rbp\n"
  "movq  -72(%rsp), %r8\n"
  "movq  -80(%rsp), %r9\n"
  "movq  -88(%rsp), %r10\n"
  "movq  -96(%rsp), %r11\n"
  "movq -104(%rsp), %r12\n"
  "movq -112(%rsp), %r13\n"
  "movq -120(%rsp), %r14\n"
  "movq -128(%rsp), %r15\n"

  "jmp " stringify(__EVILCC_REAL_ENTRY_SYMBOL) "\n"
);

#endif
