#ifndef __EVILCC_ARCH_AARCH64_SYSCALLS_H
#define __EVILCC_ARCH_AARCH64_SYSCALLS_H

#include <sys/personality.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/signal.h>
#include <sys/syscall.h>
#include <sys/types.h>

typedef unsigned short umode_t;

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

always_inline static syscall0(__NR_getegid, gid_t, __evilcc_getegid);
always_inline static syscall0(__NR_geteuid, uid_t, __evilcc_geteuid);
always_inline static syscall0(__NR_getgid, gid_t, __evilcc_getgid);
always_inline static syscall0(__NR_getuid, uid_t, __evilcc_getuid);
always_inline static syscall1(__NR_close, int, __evilcc_close, int, fd);
always_inline static syscall1(__NR_personality, int, __evilcc_personality, unsigned long, persona);
always_inline static syscall2(__NR_fchmod, int, __evilcc_fchmod, int, fd, mode_t, mode);
always_inline static syscall2(__NR_fstat, int, __evilcc_fstat, int, fd, struct stat*, statbuf);
always_inline static syscall2(__NR_kill, int, __evilcc_kill, pid_t, pid, int, sig);
always_inline static syscall3(__NR_execve, int, __evilcc_execve, const char*, pathname, const char**, argv, const char**, envp);
always_inline static syscall3(__NR_setresgid, int, __evilcc_setresgid, gid_t, rgid, gid_t, egid, gid_t, sgid);
always_inline static syscall3(__NR_setresuid, int, __evilcc_setresuid, uid_t, ruid, uid_t, euid, uid_t, suid);
always_inline static syscall3(__NR_write, ssize_t, __evilcc_write, int, fd, const void*, buf, size_t, len);
always_inline static syscall4(__NR_openat, int, __evilcc_openat, int, dfd, const char*, filename, int, flags, umode_t, mode);
always_inline static syscall5(__NR_prctl, int, __evilcc_prctl, int, option, unsigned long, arg2, unsigned long, arg3, unsigned long, arg4, unsigned long, arg5);

// https://elixir.bootlin.com/linux/v6.15.3/source/include/uapi/linux/fcntl.h#L93
#define AT_FDCWD -100

// https://elixir.bootlin.com/linux/v6.15.3/source/include/uapi/asm-generic/fcntl.h
#define O_RDWR 00000002
#define O_RDONLY 00000000

// Regular `chmod` is not available on aarch64 for some reason... so we have to
// polyfill it in.
always_inline static int __evilcc_chmod(const char* pathname, mode_t mode) {
  int fd = __evilcc_openat(AT_FDCWD, pathname, O_RDONLY, 0);
  if (fd < 0)
    return fd;

  int ret = 0;

  ret = __evilcc_fchmod(fd, mode);
  if (ret < 0)
    goto cleanup;

cleanup:
  __evilcc_close(fd);
  return ret;
}

// Regular `stat` is not available on aarch64 for some reason... so we have to
// polyfill it in.
always_inline static int __evilcc_stat(const char* pathname, struct stat* statbuf) {
  int fd = __evilcc_openat(AT_FDCWD, pathname, O_RDONLY, 0);
  if (fd < 0)
    return fd;

  int ret = 0;

  ret = __evilcc_fstat(fd, statbuf);
  if (ret < 0)
    goto cleanup;

cleanup:
  __evilcc_close(fd);
  return ret;
}

#undef AT_FDCWD
#undef O_RDWR
#undef O_RDONLY

#pragma GCC diagnostic pop

#define chmod(pathname, mode) __evilcc_chmod(pathname, mode)
#define execve(pathname, argv, envp) __evilcc_execve(pathname, argv, envp)
#define getegid() __evilcc_getegid()
#define geteuid() __evilcc_geteuid()
#define getgid() __evilcc_getgid()
#define getuid() __evilcc_getuid()
#define kill(pid, sig) __evilcc_kill(pid, sig)
#define personality(persona) __evilcc_personality(persona)
#define prctl(option, arg2, arg3, arg4, arg5) __evilcc_prctl(option, arg2, arg3, arg4, arg5)
#define setresgid(rgid, egid, sgid) __evilcc_setresgid(rgid, egid, sgid)
#define setresuid(ruid, euid, suid) __evilcc_setresuid(ruid, euid, suid)
#define stat(pathname, statbuf) __evilcc_stat(pathname, statbuf)
#define write(fd, buf, len) __evilcc_write(fd, buf, len)

#endif
