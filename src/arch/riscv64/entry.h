#ifndef __EVILCC_ARCH_RISCV64_ENTRY_H
#define __EVILCC_ARCH_RISCV64_ENTRY_H

#include "compiler.h"
#include "config.h"

used static void __evilcc_init(int argc, const char* argv[], const char* envp[]);

used always_inline noreturn static void evilcc_finish(void) {
  asm volatile(
    "j __evilcc_init_ret"
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

  // Save `x1-31`.
  "sd x1, -8(sp)\n"
  // `x2` is `sp`
  "sd x3, -24(sp)\n"
  "sd x4, -32(sp)\n"
  "sd x5, -40(sp)\n"
  "sd x6, -48(sp)\n"
  "sd x7, -56(sp)\n"
  "sd x8, -64(sp)\n"
  "sd x9, -72(sp)\n"
  "sd x10, -80(sp)\n"
  "sd x11, -88(sp)\n"
  "sd x12, -96(sp)\n"
  "sd x13, -104(sp)\n"
  "sd x14, -112(sp)\n"
  "sd x15, -120(sp)\n"
  "sd x16, -128(sp)\n"
  "sd x17, -136(sp)\n"
  "sd x18, -144(sp)\n"
  "sd x19, -152(sp)\n"
  "sd x20, -160(sp)\n"
  "sd x21, -168(sp)\n"
  "sd x22, -176(sp)\n"
  "sd x23, -184(sp)\n"
  "sd x24, -192(sp)\n"
  "sd x25, -200(sp)\n"
  "sd x26, -208(sp)\n"
  "sd x27, -216(sp)\n"
  "sd x28, -224(sp)\n"
  "sd x29, -232(sp)\n"
  "sd x30, -240(sp)\n"
  "sd x31, -248(sp)\n"

  // Save `sp`.
  "la t0, __evilcc_stack\n"
  "sd sp, (t0)\n"
  "mv t0, sp\n"
  "addi sp, sp, -248\n"

  /////////////////////////////////////////////////////////////////////////////

  "ld a0, 0(t0)\n"   // argc
  "addi a1, t0, 8\n" // argv
  "slli a2, a0, 3\n"
  "addi a2, a2, 16\n"
  "add a2, t0, a2\n" // envp
  "j __evilcc_init\n"

  ".local __evilcc_init_ret\n"
  "__evilcc_init_ret:\n"

  /////////////////////////////////////////////////////////////////////////////

  // Restore `sp`.
  "la t0, __evilcc_stack\n"
  "ld sp, (t0)\n"

  // Restore `x1-31`.
  "ld x1, -8(sp)\n"
  // `x2` is `sp`
  "ld x3, -24(sp)\n"
  "ld x4, -32(sp)\n"
  "ld x5, -40(sp)\n"
  "ld x6, -48(sp)\n"
  "ld x7, -56(sp)\n"
  "ld x8, -64(sp)\n"
  "ld x9, -72(sp)\n"
  "ld x10, -80(sp)\n"
  "ld x11, -88(sp)\n"
  "ld x12, -96(sp)\n"
  "ld x13, -104(sp)\n"
  "ld x14, -112(sp)\n"
  "ld x15, -120(sp)\n"
  "ld x16, -128(sp)\n"
  "ld x17, -136(sp)\n"
  "ld x18, -144(sp)\n"
  "ld x19, -152(sp)\n"
  "ld x20, -160(sp)\n"
  "ld x21, -168(sp)\n"
  "ld x22, -176(sp)\n"
  "ld x23, -184(sp)\n"
  "ld x24, -192(sp)\n"
  "ld x25, -200(sp)\n"
  "ld x26, -208(sp)\n"
  "ld x27, -216(sp)\n"
  "ld x28, -224(sp)\n"
  "ld x29, -232(sp)\n"
  "ld x30, -240(sp)\n"
  "ld x31, -248(sp)\n"

  "j " stringify(__EVILCC_REAL_ENTRY_SYMBOL) "\n"
);

#endif
