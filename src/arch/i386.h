#ifndef __EVILCC_ARCH_i386
#define __EVILCC_ARCH_i386

#include <sys/personality.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>

#include "../common.h"

#define syscall0(num, ret, name) \
  ret name(void) { \
    ret result;\
\
    asm volatile(\
      "int $0x80"\
      : "=a"(result)\
      : "a"(num)\
      : "memory"\
    );\
\
    return result;\
  }

#define syscall1(num, ret, name, arg1_t, arg1)\
  ret name(arg1_t arg1) { \
    ret result;\
\
    asm volatile(\
      "int $0x80"\
      : "=a"(result)\
      : "a"(num), "b"(arg1)\
      : "memory"\
    );\
\
    return result;\
  }

#define syscall2(num, ret, name, arg1_t, arg1, arg2_t, arg2)\
  ret name(arg1_t arg1, arg2_t arg2) {\
    ret result;\
\
    asm volatile(\
      "int $0x80"\
      : "=a"(result)\
      : "a"(num), "b"(arg1), "c"(arg2)\
      : "memory"\
    );\
    \
    return result;\
  }

#define syscall3(num, ret, name, arg1_t, arg1, arg2_t, arg2, arg3_t, arg3)\
  ret name(arg1_t arg1, arg2_t arg2, arg3_t arg3) {\
    ret result;\
    \
    asm volatile(\
      "int $0x80"\
      : "=a"(result)\
      : "a"(num), "b"(arg1), "c"(arg2), "d"(arg3)\
      : "memory"\
    );\
    \
    return result;\
  }

#define syscall5(num, ret, name, arg1_t, arg1, arg2_t, arg2, arg3_t, arg3, arg4_t, arg4, arg5_t, arg5)\
  ret name(arg1_t arg1, arg2_t arg2, arg3_t arg3, arg4_t arg4, arg5_t arg5) {\
    ret result;\
    \
    asm volatile(\
      "int $0x80"\
      : "=a"(result)\
      : "a"(num), "b"(arg1), "c"(arg2), "d"(arg3), "S"(arg4), "D"(arg5)\
      : "memory"\
    );\
    \
    return result;\
  }

always_inline static syscall0(__NR_getegid, gid_t, _getegid);
always_inline static syscall0(__NR_geteuid, uid_t, _geteuid);
always_inline static syscall0(__NR_getgid, gid_t, _getgid);
always_inline static syscall0(__NR_getuid, uid_t, _getuid);
always_inline static syscall1(__NR_personality, int, _personality, unsigned long, persona);
always_inline static syscall2(__NR_chmod, int, _chmod, const char*, pathname, mode_t, mode);
always_inline static syscall2(__NR_stat, int, _stat, const char*, pathname, struct stat*, statbuf);
always_inline static syscall3(__NR_execve, int, _execve, const char*, pathname, const char**, argv, const char**, envp);
always_inline static syscall3(__NR_setresgid, int, _setresgid, gid_t, rgid, gid_t, egid, gid_t, sgid);
always_inline static syscall3(__NR_setresuid, int, _setresuid, uid_t, ruid, uid_t, euid, uid_t, suid);
always_inline static syscall5(__NR_prctl, int, _prctl, int, option, unsigned long, arg2, unsigned long, arg3, unsigned long, arg4, unsigned long, arg5);

#define getegid() _getegid()
#define geteuid() _geteuid()
#define getgid() _getgid()
#define getuid() _getuid()
#define personality(persona) _personality(persona)
#define chmod(pathname, mode) _chmod(pathname, mode)
#define stat(pathname, statbuf) _stat(pathname, statbuf)
#define execve(pathname, argv, envp) _execve(pathname, argv, envp)
#define setresgid(rgid, egid, sgid) _setresgid(rgid, egid, sgid)
#define setresuid(ruid, euid, suid) _setresuid(ruid, euid, suid)
#define prctl(option, arg2, arg3, arg4, arg5) _prctl(option, arg2, arg3, arg4, arg5)

always_inline static void die(void) {
  asm volatile("int3");
}

asm (
  ".text\n"
  ".globl " stringify(__EVILCC_ENTRY_SYMBOL) "\n"
  stringify(__EVILCC_ENTRY_SYMBOL) ":\n"
  "pusha\n"
  "pushfl\n"

  "lea 36(%esp), %ebx\n"
  "sub $12, %esp\n"
  "mov (%ebx), %eax\n"
  "mov %eax, 0(%esp)\n" // argc
  "inc %eax\n"
  "lea 4(%ebx, %eax, 4), %eax\n"
  "mov %eax, 8(%esp)\n" // envp
  "lea 4(%ebx), %eax\n"
  "mov %eax, 4(%esp)\n" // argv
  "call __evilcc_init\n"
  "add $12, %esp\n"

  "popfl\n"
  "popa\n"

  "jmp " stringify(__EVILCC_REAL_ENTRY_SYMBOL) "\n"
);

#endif
