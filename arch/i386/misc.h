#ifndef __EVILCC_ARCH_I386_MISC_H
#define __EVILCC_ARCH_I386_MISC_H

#include "compiler.h"

always_inline static void die(void) {
  asm volatile("ud2");
}

#endif
