#ifndef __EVILCC_ARCH_RISCV32_MISC_H
#define __EVILCC_ARCH_RISCV32_MISC_H

#include "compiler.h"

always_inline static void die(void) {
  asm volatile("ebreak");
}

#endif
