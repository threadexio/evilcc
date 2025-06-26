#ifndef __EVILCC_ARCH_X86_64_SYSCALLS_H
#define __EVILCC_ARCH_X86_64_SYSCALLS_H

#include <sys/personality.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/signal.h>
#include <sys/syscall.h>
#include <sys/types.h>

#include "compiler.h"

#define syscall0(num, ret, name)                                              \
  ret name(void) {                                                            \
    ret result;                                                               \
                                                                              \
    asm volatile(                                                             \
      "syscall"                                                               \
      : "=a"(result)                                                          \
      : "a"(num)                                                              \
      : "rcx", "r11", "memory"                                                \
    );                                                                        \
                                                                              \
    return result;                                                            \
  }

#define syscall1(num, ret, name, arg1_t, arg1)                                \
  ret name(arg1_t arg1) {                                                     \
    ret result;                                                               \
                                                                              \
    asm volatile(                                                             \
      "syscall"                                                               \
      : "=a"(result)                                                          \
      : "a"(num), "D"(arg1)                                                   \
      : "rcx", "r11", "memory"                                                \
    );                                                                        \
                                                                              \
    return result;                                                            \
  }

#define syscall2(num, ret, name, arg1_t, arg1, arg2_t, arg2)                  \
  ret name(arg1_t arg1, arg2_t arg2) {                                        \
    ret result;                                                               \
                                                                              \
    asm volatile(                                                             \
      "syscall"                                                               \
      : "=a"(result)                                                          \
      : "a"(num), "D"(arg1), "S"(arg2)                                        \
      : "rcx", "r11", "memory"                                                \
    );                                                                        \
                                                                              \
    return result;                                                            \
  }


#define syscall3(num, ret, name, arg1_t, arg1, arg2_t, arg2, arg3_t, arg3)    \
  ret name(arg1_t arg1, arg2_t arg2, arg3_t arg3) {                           \
    ret result;                                                               \
                                                                              \
    asm volatile(                                                             \
      "syscall"                                                               \
      : "=a"(result)                                                          \
      : "a"(num), "D"(arg1), "S"(arg2), "d"(arg3)                             \
      : "rcx", "r11", "memory"                                                \
    );                                                                        \
                                                                              \
    return result;                                                            \
  }

#define syscall5(num, ret, name, arg1_t, arg1, arg2_t, arg2, arg3_t, arg3, arg4_t, arg4, arg5_t, arg5) \
  ret name(arg1_t arg1, arg2_t arg2, arg3_t arg3, arg4_t arg4, arg5_t arg5) {                          \
    int result;                                                                                        \
                                                                                                       \
    register long r10 asm("r10") = arg4;                                                               \
    register long r8 asm("r8") = arg5;                                                                 \
                                                                                                       \
    asm volatile(                                                                                      \
      "syscall"                                                                                        \
      : "=a"(result)                                                                                   \
      : "a"(num), "D"(arg1), "S"(arg2), "d"(arg3), "r"(r10), "r"(r8)                                   \
      : "rcx", "r11", "memory"                                                                         \
    );                                                                                                 \
                                                                                                       \
    return result;                                                                                     \
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

always_inline static syscall0(__NR_getegid, gid_t, __evilcc_getegid);
always_inline static syscall0(__NR_geteuid, uid_t, __evilcc_geteuid);
always_inline static syscall0(__NR_getgid, gid_t, __evilcc_getgid);
always_inline static syscall0(__NR_getuid, uid_t, __evilcc_getuid);
always_inline static syscall1(__NR_personality, int, __evilcc_personality, unsigned long, persona);
always_inline static syscall2(__NR_chmod, int, __evilcc_chmod, const char*, pathname, mode_t, mode);
always_inline static syscall2(__NR_kill, int, __evilcc_kill, pid_t, pid, int, sig);
always_inline static syscall2(__NR_stat, int, __evilcc_stat, const char*, pathname, struct stat*, statbuf);
always_inline static syscall3(__NR_execve, int, __evilcc_execve, const char*, pathname, const char**, argv, const char**, envp);
always_inline static syscall3(__NR_setresgid, int, __evilcc_setresgid, gid_t, rgid, gid_t, egid, gid_t, sgid);
always_inline static syscall3(__NR_setresuid, int, __evilcc_setresuid, uid_t, ruid, uid_t, euid, uid_t, suid);
always_inline static syscall3(__NR_write, ssize_t, __evilcc_write, int, fd, const void*, buf, size_t, len);
always_inline static syscall5(__NR_prctl, int, __evilcc_prctl, int, option, unsigned long, arg2, unsigned long, arg3, unsigned long, arg4, unsigned long, arg5);

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
