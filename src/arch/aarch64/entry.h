#ifndef __EVILCC_ARCH_AARCH64_ENTRY_H
#define __EVILCC_ARCH_AARCH64_ENTRY_H

#include "compiler.h"
#include "config.h"

used static void __evilcc_init(int argc, const char* argv[], const char* envp[]);

always_inline noreturn static void evilcc_finish(void) {
  asm volatile(
    "b __evilcc_init_ret\n"
    :::
  );

  unreachable();
}

asm (
  ".data\n"
  ".local __evilcc_context\n"
  "__evilcc_stack: .quad 0\n"

  ".text\n"
  ".globl " stringify(__EVILCC_ENTRY_SYMBOL) "\n"
  stringify(__EVILCC_ENTRY_SYMBOL) ":\n"

  /////////////////////////////////////////////////////////////////////////////

  // Save `x0-30`.
  "stp x0, x1,   [sp, #-16]!\n"
  "stp x2, x3,   [sp, #-16]!\n"
  "stp x4, x5,   [sp, #-16]!\n"
  "stp x6, x7,   [sp, #-16]!\n"
  "stp x8, x9,   [sp, #-16]!\n"
  "stp x10, x11, [sp, #-16]!\n"
  "stp x12, x13, [sp, #-16]!\n"
  "stp x14, x15, [sp, #-16]!\n"
  "stp x16, x17, [sp, #-16]!\n"
  "stp x18, x19, [sp, #-16]!\n"
  "stp x20, x21, [sp, #-16]!\n"
  "stp x22, x23, [sp, #-16]!\n"
  "stp x24, x25, [sp, #-16]!\n"
  "stp x26, x27, [sp, #-16]!\n"
  "stp x28, x29, [sp, #-16]!\n"
  "str x30,      [sp, #-8]!\n"

  // Save `pstate`.
  "mrs x1, nzcv\n"
  "str x1, [sp, #-8]!\n"

  // Save FPU state.
  "mrs x1, fpcr\n"
  "str x1, [sp, #-8]!\n"
  "mrs x1, fpsr\n"
  "str x1, [sp, #-8]!\n"

  // Save FP registers.
  "stp q0, q1,   [sp, #-32]!\n"
  "stp q2, q3,   [sp, #-32]!\n"
  "stp q4, q5,   [sp, #-32]!\n"
  "stp q6, q7,   [sp, #-32]!\n"
  "stp q8, q9,   [sp, #-32]!\n"
  "stp q10, q11, [sp, #-32]!\n"
  "stp q12, q13, [sp, #-32]!\n"
  "stp q14, q15, [sp, #-32]!\n"
  "stp q16, q17, [sp, #-32]!\n"
  "stp q18, q19, [sp, #-32]!\n"
  "stp q20, q21, [sp, #-32]!\n"
  "stp q22, q23, [sp, #-32]!\n"
  "stp q24, q25, [sp, #-32]!\n"
  "stp q26, q27, [sp, #-32]!\n"
  "stp q28, q29, [sp, #-32]!\n"
  "stp q30, q31, [sp, #-32]!\n"

  // Save `sp`.
  "adr x0, __evilcc_stack\n"
  "mov x3, sp\n"
  "str x3, [x0]\n"

  /////////////////////////////////////////////////////////////////////////////

  // Calculate `sp` as it was before we saved the CPU context.
  "add x3, x3, #784\n"

  "ldr x0, [x3], #8\n"       // argc
  "mov x1, x3\n"             // argv
  "add x2, x3, x0, lsl #3\n"
  "add x2, x2, #8\n"         // envp
  "b __evilcc_init\n"

  ".local __evilcc_init_ret\n"
  "__evilcc_init_ret:\n"

  /////////////////////////////////////////////////////////////////////////////

  // Restore `sp`.
  "adr x0, __evilcc_stack\n"
  "ldr x0, [x0]\n"
  "mov sp, x0\n"

  // Restore FP registers.
  "ldp q30, q31, [sp], #32\n"
  "ldp q28, q29, [sp], #32\n"
  "ldp q26, q27, [sp], #32\n"
  "ldp q24, q25, [sp], #32\n"
  "ldp q22, q23, [sp], #32\n"
  "ldp q20, q21, [sp], #32\n"
  "ldp q18, q19, [sp], #32\n"
  "ldp q16, q17, [sp], #32\n"
  "ldp q14, q15, [sp], #32\n"
  "ldp q12, q13, [sp], #32\n"
  "ldp q10, q11, [sp], #32\n"
  "ldp q8, q9,   [sp], #32\n"
  "ldp q6, q7,   [sp], #32\n"
  "ldp q4, q5,   [sp], #32\n"
  "ldp q2, q3,   [sp], #32\n"
  "ldp q0, q1,   [sp], #32\n"

  // Restore FPU state.
  "ldr x1, [sp], #8\n"
  "msr fpsr, x1\n"
  "ldr x1, [sp], #8\n"
  "msr fpcr, x1\n"

  // Restore `pstate`.
  "ldr x1, [sp], #8\n"
  "msr nzcv, x1\n"

  // Restore `x0-30`.
  "ldr x30,      [sp], #8\n"
  "ldp x28, x29, [sp], #16\n"
  "ldp x26, x27, [sp], #16\n"
  "ldp x24, x25, [sp], #16\n"
  "ldp x22, x23, [sp], #16\n"
  "ldp x20, x21, [sp], #16\n"
  "ldp x18, x19, [sp], #16\n"
  "ldp x16, x17, [sp], #16\n"
  "ldp x14, x15, [sp], #16\n"
  "ldp x12, x13, [sp], #16\n"
  "ldp x10, x11, [sp], #16\n"
  "ldp x8, x9,   [sp], #16\n"
  "ldp x6, x7,   [sp], #16\n"
  "ldp x4, x5,   [sp], #16\n"
  "ldp x2, x3,   [sp], #16\n"
  "ldp x0, x1,   [sp], #16\n"

  /////////////////////////////////////////////////////////////////////////////

  "b " stringify(__EVILCC_REAL_ENTRY_SYMBOL) "\n"
);

#endif
