#ifndef __EVILCC_ARCH_RISCV32_ENTRY_H
#define __EVILCC_ARCH_RISCV32_ENTRY_H

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
  "sw x1,    -4(sp)\n"
  // `x2` is `sp`
  "sw x3,   -8(sp)\n"
  "sw x4,   -12(sp)\n"
  "sw x5,   -16(sp)\n"
  "sw x6,   -20(sp)\n"
  "sw x7,   -24(sp)\n"
  "sw x8,   -28(sp)\n"
  "sw x9,   -32(sp)\n"
  "sw x10,  -36(sp)\n"
  "sw x11,  -40(sp)\n"
  "sw x12,  -44(sp)\n"
  "sw x13,  -48(sp)\n"
  "sw x14,  -52(sp)\n"
  "sw x15,  -56(sp)\n"
  "sw x16,  -60(sp)\n"
  "sw x17,  -64(sp)\n"
  "sw x18,  -68(sp)\n"
  "sw x19,  -72(sp)\n"
  "sw x20,  -76(sp)\n"
  "sw x21,  -80(sp)\n"
  "sw x22,  -84(sp)\n"
  "sw x23,  -88(sp)\n"
  "sw x24,  -92(sp)\n"
  "sw x25,  -96(sp)\n"
  "sw x26, -100(sp)\n"
  "sw x27, -104(sp)\n"
  "sw x28, -108(sp)\n"
  "sw x29, -112(sp)\n"
  "sw x30, -116(sp)\n"
  "sw x31, -120(sp)\n"

  // Save `sp`.
  "la t0, __evilcc_stack\n"
  "sw sp, (t0)\n"
  "mv t0, sp\n"
  "addi sp, sp, -120\n"

  /////////////////////////////////////////////////////////////////////////////

  "lw a0, 0(t0)\n"   // argc
  "addi a1, t0, 4\n" // argv
  "slli a2, a0, 2\n"
  "addi a2, a2, 8\n"
  "add a2, t0, a2\n" // envp
  "j __evilcc_init\n"

  ".local __evilcc_init_ret\n"
  "__evilcc_init_ret:\n"

  /////////////////////////////////////////////////////////////////////////////

  // Restore `sp`.
  "la t0, __evilcc_stack\n"
  "lw sp, (t0)\n"

  // Restore `x1-31`.
  "lw x1,    -4(sp)\n"
  // `x2` is `sp`
  "lw x3,   -8(sp)\n"
  "lw x4,   -12(sp)\n"
  "lw x5,   -16(sp)\n"
  "lw x6,   -20(sp)\n"
  "lw x7,   -24(sp)\n"
  "lw x8,   -28(sp)\n"
  "lw x9,   -32(sp)\n"
  "lw x10,  -36(sp)\n"
  "lw x11,  -40(sp)\n"
  "lw x12,  -44(sp)\n"
  "lw x13,  -48(sp)\n"
  "lw x14,  -52(sp)\n"
  "lw x15,  -56(sp)\n"
  "lw x16,  -60(sp)\n"
  "lw x17,  -64(sp)\n"
  "lw x18,  -68(sp)\n"
  "lw x19,  -72(sp)\n"
  "lw x20,  -76(sp)\n"
  "lw x21,  -80(sp)\n"
  "lw x22,  -84(sp)\n"
  "lw x23,  -88(sp)\n"
  "lw x24,  -92(sp)\n"
  "lw x25,  -96(sp)\n"
  "lw x26, -100(sp)\n"
  "lw x27, -104(sp)\n"
  "lw x28, -108(sp)\n"
  "lw x29, -112(sp)\n"
  "lw x30, -116(sp)\n"
  "lw x31, -120(sp)\n"

  "j " stringify(__EVILCC_REAL_ENTRY_SYMBOL) "\n"
);

#endif
