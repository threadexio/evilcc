#ifndef __EVILCC_LOG_H
#define __EVILCC_LOG_H

#include "compiler.h"
#include "arch.h"
#include "polyfill.h"

#define log(msg) _log(msg, __FILE__, stringify(__LINE__))
#define fatal(msg) (log(msg), die())

always_inline static void _log(const char* msg, const char* file, const char* line) {
#if defined(__EVILCC_DEBUG)
  const int fd = 2;

  write(fd, "[evilcc ", 8);
  write(fd, file, strlen(file));
  write(fd, ":", 1);
  write(fd, line, strlen(line));
  write(fd, "] ", 2);
  write(fd, msg, strlen(msg));
  write(fd, "\n", 1);
#else
  (void)msg; (void)file; (void)line;
#endif
}

#endif
