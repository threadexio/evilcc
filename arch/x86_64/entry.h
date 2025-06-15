#ifndef __EVILCC_ARCH_X86_64_ENTRY_H
#define __EVILCC_ARCH_X86_64_ENTRY_H

#include "compiler.h"
#include "config.h"

used static void __evilcc_init(int argc, const char* argv[], const char* envp[]);

asm (
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

  "lea 128(%rsp), %rbx\n"
  "mov 0(%rbx), %rdi\n"           // argc
  "lea 8(%rbx), %rsi\n"           // argv
  "lea 16(%rbx, %rdi, 8), %rdx\n" // envp
  "call __evilcc_init\n"

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
