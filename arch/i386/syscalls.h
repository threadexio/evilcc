#ifndef __EVILCC_ARCH_I386_SYSCALLS_H
#define __EVILCC_ARCH_I386_SYSCALLS_H

#include <sys/personality.h>
#include <sys/prctl.h>
#include <sys/signal.h>
#include <sys/syscall.h>
#include <sys/types.h>

#include "compiler.h"

#define S_IFMT  00170000
#define S_IFSOCK 0140000
#define S_IFLNK	 0120000
#define S_IFREG  0100000
#define S_IFBLK  0060000
#define S_IFDIR  0040000
#define S_IFCHR  0020000
#define S_IFIFO  0010000
#define S_ISUID  0004000
#define S_ISGID  0002000
#define S_ISVTX  0001000

struct stat {
  unsigned long  st_dev;
  unsigned long  st_ino;
  unsigned short st_mode;
  unsigned short st_nlink;
  unsigned short st_uid;
  unsigned short st_gid;
  unsigned long  st_rdev;
  unsigned long  st_size;
  unsigned long  st_blksize;
  unsigned long  st_blocks;
  unsigned long  st_atime;
  unsigned long  st_atime_nsec;
  unsigned long  st_mtime;
  unsigned long  st_mtime_nsec;
  unsigned long  st_ctime;
  unsigned long  st_ctime_nsec;
  unsigned long  __unused4;
  unsigned long  __unused5;
};

#define syscall0(num, ret, name)                                              \
  ret name(void) {                                                            \
    ret result;                                                               \
                                                                              \
    asm volatile(                                                             \
      "int $0x80"                                                             \
      : "=a"(result)                                                          \
      : "a"(num)                                                              \
      : "memory"                                                              \
    );                                                                        \
                                                                              \
    return result;                                                            \
  }

#define syscall1(num, ret, name, arg1_t, arg1)                                \
  ret name(arg1_t arg1) {                                                     \
    ret result;                                                               \
                                                                              \
    asm volatile(                                                             \
      "int $0x80"                                                             \
      : "=a"(result)                                                          \
      : "a"(num), "b"(arg1)                                                   \
      : "memory"                                                              \
    );                                                                        \
                                                                              \
    return result;                                                            \
  }

#define syscall2(num, ret, name, arg1_t, arg1, arg2_t, arg2)\
  ret name(arg1_t arg1, arg2_t arg2) {                                        \
    ret result;                                                               \
                                                                              \
    asm volatile(                                                             \
      "int $0x80"                                                             \
      : "=a"(result)                                                          \
      : "a"(num), "b"(arg1), "c"(arg2)                                        \
      : "memory"                                                              \
    );                                                                        \
                                                                              \
    return result;                                                            \
  }

#define syscall3(num, ret, name, arg1_t, arg1, arg2_t, arg2, arg3_t, arg3)    \
  ret name(arg1_t arg1, arg2_t arg2, arg3_t arg3) {                           \
    ret result;                                                               \
                                                                              \
    asm volatile(                                                             \
      "int $0x80"                                                             \
      : "=a"(result)                                                          \
      : "a"(num), "b"(arg1), "c"(arg2), "d"(arg3)                             \
      : "memory"                                                              \
    );                                                                        \
                                                                              \
    return result;                                                            \
  }

#define syscall5(num, ret, name, arg1_t, arg1, arg2_t, arg2, arg3_t, arg3, arg4_t, arg4, arg5_t, arg5) \
  ret name(arg1_t arg1, arg2_t arg2, arg3_t arg3, arg4_t arg4, arg5_t arg5) {                          \
    ret result;                                                                                        \
                                                                                                       \
    asm volatile(                                                                                      \
      "int $0x80"                                                                                      \
      : "=a"(result)                                                                                   \
      : "a"(num), "b"(arg1), "c"(arg2), "d"(arg3), "S"(arg4), "D"(arg5)                                \
      : "memory"                                                                                       \
    );                                                                                                 \
                                                                                                       \
    return result;                                                                                     \
  }

always_inline static syscall0(__NR_getegid, gid_t, _getegid);
always_inline static syscall0(__NR_geteuid, uid_t, _geteuid);
always_inline static syscall0(__NR_getgid, gid_t, _getgid);
always_inline static syscall0(__NR_getuid, uid_t, _getuid);
always_inline static syscall1(__NR_personality, int, _personality, unsigned long, persona);
always_inline static syscall2(__NR_chmod, int, _chmod, const char*, pathname, mode_t, mode);
always_inline static syscall2(__NR_kill, int, _kill, pid_t, pid, int, sig);
always_inline static syscall2(__NR_stat, int, _stat, const char*, pathname, struct stat*, statbuf);
always_inline static syscall3(__NR_execve, int, _execve, const char*, pathname, const char**, argv, const char**, envp);
always_inline static syscall3(__NR_setresgid, int, _setresgid, gid_t, rgid, gid_t, egid, gid_t, sgid);
always_inline static syscall3(__NR_setresuid, int, _setresuid, uid_t, ruid, uid_t, euid, uid_t, suid);
always_inline static syscall3(__NR_write, ssize_t, _write, int, fd, const void*, buf, size_t, len);
always_inline static syscall5(__NR_prctl, int, _prctl, int, option, unsigned long, arg2, unsigned long, arg3, unsigned long, arg4, unsigned long, arg5);

#define chmod(pathname, mode) _chmod(pathname, mode)
#define execve(pathname, argv, envp) _execve(pathname, argv, envp)
#define getegid() _getegid()
#define geteuid() _geteuid()
#define getgid() _getgid()
#define getuid() _getuid()
#define kill(pid, sig) _kill(pid, sig)
#define personality(persona) _personality(persona)
#define prctl(option, arg2, arg3, arg4, arg5) _prctl(option, arg2, arg3, arg4, arg5)
#define setresgid(rgid, egid, sgid) _setresgid(rgid, egid, sgid)
#define setresuid(ruid, euid, suid) _setresuid(ruid, euid, suid)
#define stat(pathname, statbuf) _stat(pathname, statbuf)
#define write(fd, buf, len) _write(fd, buf, len)

#endif
