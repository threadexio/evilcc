#ifndef __EVILCC_POLYFILL_H
#define __EVILCC_POLYFILL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "compiler.h"

always_inline static size_t strlen(const char* s) {
  size_t i = 0;
  while (*s++ != '\0') i++;
  return i;
}

#endif
