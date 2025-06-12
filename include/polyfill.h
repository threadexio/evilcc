#ifndef __EVILCC_POLYFILL_H
#define __EVILCC_POLYFILL_H

#include <alloca.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "arch.h"
#include "compiler.h"

#define alloca_zeroed(size) ({ \
  size_t _size = (size); \
  void* mem = alloca(_size); \
  memset(mem, 0, _size); \
  mem; \
})

always_inline static size_t strlen(const char* s) {
  size_t i = 0;
  while (*s++ != '\0') i++;
  return i;
}

always_inline static bool streq(const char* restrict s1, const char* restrict s2) {
  if (s1 == NULL || s2 == NULL)
    die();

  for (;; s1++, s2++) {
    if (*s1 == '\0' && *s2 == '\0')
      return true;

    if (*s1 != *s2)
      return false;
  }
}

always_inline static bool strneq(const char* restrict s1, const char* restrict s2, size_t n) {
  if (s1 == NULL || s2 == NULL)
    die();

  for (size_t i = 0; i < n; i++, s1++, s2++) {
    if (*s1 == '\0' && *s2 == '\0')
      return true;

    if (*s1 != *s2)
      return false;
  }

  return true;
}

always_inline static size_t array_len(const char** array) {
  size_t len = 0;
  while (*array++ != NULL) len++;
  return len;
}

#endif
