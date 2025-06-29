#ifndef __EVILCC_ARCH_AARCH64_SYSCALLS_H
#define __EVILCC_ARCH_AARCH64_SYSCALLS_H

#include <sys/signal.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <linux/fcntl.h>
#include <linux/personality.h>
#include <linux/prctl.h>
#include <linux/stat.h>
#include <linux/types.h>
#include <linux/unistd.h>

#include "compiler.h"

#define syscall0(num, ret, name)                                              \
  ret name(void) {                                                            \
    register long x8 __asm__("x8") = (num);                                   \
    register long x0 __asm__("x0") = 0;                                       \
                                                                              \
    asm volatile(                                                             \
      "svc #0"                                                                \
      : "=r"(x0)                                                              \
      : "r"(x8), "0"(x0)                                                      \
      : "memory"                                                              \
    );                                                                        \
                                                                              \
    return (ret)x0;                                                           \
  }

#define syscall1(num, ret, name, arg1_t, arg1)                                \
  ret name(arg1_t arg1) {                                                     \
    register long x8 __asm__("x8") = (num);                                   \
    register long x0 __asm__("x0") = (long)(arg1);                            \
                                                                              \
    asm volatile(                                                             \
      "svc #0"                                                                \
      : "=r"(x0)                                                              \
      : "r"(x8), "0"(x0)                                                      \
      : "memory"                                                              \
    );                                                                        \
                                                                              \
    return (ret)x0;                                                           \
  }

#define syscall2(num, ret, name, arg1_t, arg1, arg2_t, arg2)                  \
  ret name(arg1_t arg1, arg2_t arg2) {                                        \
    register long x8 __asm__("x8") = (num);                                   \
    register long x0 __asm__("x0") = (long)(arg1);                            \
    register long x1 __asm__("x1") = (long)(arg2);                            \
                                                                              \
    asm volatile(                                                             \
      "svc #0"                                                                \
      : "=r"(x0)                                                              \
      : "r"(x8), "0"(x0), "r"(x1)                                             \
      : "memory"                                                              \
    );                                                                        \
                                                                              \
    return (ret)x0;                                                           \
  }


#define syscall3(num, ret, name, arg1_t, arg1, arg2_t, arg2, arg3_t, arg3)    \
  ret name(arg1_t arg1, arg2_t arg2, arg3_t arg3) {                           \
    register long x8 __asm__("x8") = (num);                                   \
    register long x0 __asm__("x0") = (long)(arg1);                            \
    register long x1 __asm__("x1") = (long)(arg2);                            \
    register long x2 __asm__("x2") = (long)(arg3);                            \
                                                                              \
    asm volatile(                                                             \
      "svc #0"                                                                \
      : "=r"(x0)                                                              \
      : "r"(x8), "0"(x0), "r"(x1), "r"(x2)                                    \
      : "memory"                                                              \
    );                                                                        \
                                                                              \
    return (ret)x0;                                                           \
  }

#define syscall4(num, ret, name, arg1_t, arg1, arg2_t, arg2, arg3_t, arg3, arg4_t, arg4) \
  ret name(arg1_t arg1, arg2_t arg2, arg3_t arg3, arg4_t arg4) {                         \
    register long x8 __asm__("x8") = (num);                                              \
    register long x0 __asm__("x0") = (long)(arg1);                                       \
    register long x1 __asm__("x1") = (long)(arg2);                                       \
    register long x2 __asm__("x2") = (long)(arg3);                                       \
    register long x3 __asm__("x3") = (long)(arg4);                                       \
                                                                                         \
    asm volatile(                                                                        \
      "svc #0"                                                                           \
      : "=r"(x0)                                                                         \
      : "r"(x8), "0"(x0), "r"(x1), "r"(x2), "r"(x3)                                      \
      : "memory"                                                                         \
    );                                                                                   \
                                                                                         \
    return (ret)x0;                                                                      \
  }

#define syscall5(num, ret, name, arg1_t, arg1, arg2_t, arg2, arg3_t, arg3, arg4_t, arg4, arg5_t, arg5) \
  ret name(arg1_t arg1, arg2_t arg2, arg3_t arg3, arg4_t arg4, arg5_t arg5) {                          \
    register long x8 __asm__("x8") = (num);                                                            \
    register long x0 __asm__("x0") = (long)(arg1);                                                     \
    register long x1 __asm__("x1") = (long)(arg2);                                                     \
    register long x2 __asm__("x2") = (long)(arg3);                                                     \
    register long x3 __asm__("x3") = (long)(arg4);                                                     \
    register long x4 __asm__("x4") = (long)(arg5);                                                     \
                                                                                                       \
    asm volatile(                                                                                      \
      "svc #0"                                                                                         \
      : "=r"(x0)                                                                                       \
      : "r"(x8), "0"(x0), "r"(x1), "r"(x2), "r"(x3), "r"(x4)                                           \
      : "memory"                                                                                       \
    );                                                                                                 \
                                                                                                       \
    return (ret)x0;                                                                                    \
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

///////////////////////////////////////////////////////////////////////////////

always_inline static syscall0(__NR_getegid, gid_t, __evilcc_getegid);
always_inline static syscall0(__NR_geteuid, uid_t, __evilcc_geteuid);
always_inline static syscall0(__NR_getgid, gid_t, __evilcc_getgid);
always_inline static syscall0(__NR_getuid, uid_t, __evilcc_getuid);

#define getegid() __evilcc_getegid()
#define geteuid() __evilcc_geteuid()
#define getgid()  __evilcc_getgid()
#define getuid()  __evilcc_getuid()

///////////////////////////////////////////////////////////////////////////////

always_inline static syscall1(__NR_personality, int, __evilcc_personality, unsigned long, persona);

#define personality(...) __evilcc_personality(__VA_ARGS__)

///////////////////////////////////////////////////////////////////////////////

always_inline static syscall2(__NR_kill, int, __evilcc_kill, pid_t, pid, int, sig);

#define kill(...) __evilcc_kill(__VA_ARGS__)

///////////////////////////////////////////////////////////////////////////////

always_inline static syscall3(__NR_execve, int, __evilcc_execve, const char*, pathname, const char**, argv, const char**, envp);
always_inline static syscall3(__NR_fchmodat, int, __evilcc_fchmodat, int, dfd, const char*, path, mode_t, mode);
always_inline static syscall3(__NR_setresgid, int, __evilcc_setresgid, gid_t, rgid, gid_t, egid, gid_t, sgid);
always_inline static syscall3(__NR_setresuid, int, __evilcc_setresuid, uid_t, ruid, uid_t, euid, uid_t, suid);
always_inline static syscall3(__NR_write, ssize_t, __evilcc_write, int, fd, const void*, buf, size_t, len);

#define execve(...)    __evilcc_execve(__VA_ARGS__)
#define fchmodat(...)  __evilcc_fchmodat(__VA_ARGS__)
#define setresgid(...) __evilcc_setresgid(__VA_ARGS__)
#define setresuid(...) __evilcc_setresuid(__VA_ARGS__)
#define write(...)     __evilcc_write(__VA_ARGS__)

///////////////////////////////////////////////////////////////////////////////

always_inline static syscall5(__NR_prctl, int, __evilcc_prctl, int, option, unsigned long, arg2, unsigned long, arg3, unsigned long, arg4, unsigned long, arg5);
always_inline static syscall5(__NR_statx, int, __evilcc_statx, int, dfd, const char*, path, unsigned int, flags, unsigned int, mask, struct statx*, buffer);

#define prctl(...) __evilcc_prctl(__VA_ARGS__)
#define statx(...) __evilcc_statx(__VA_ARGS__)

///////////////////////////////////////////////////////////////////////////////

#pragma GCC diagnostic pop
#endif
