#ifndef __EVILCC_ARCH_x86_64
#define __EVILCC_ARCH_x86_64

#include <sys/personality.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>

#include "../common.h"

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

naked void __EVILCC_ENTRY_SYMBOL(void) {
  asm volatile(
    "movq %%rax,   -8(%%rsp)\n"
    "movq %%rbx,  -16(%%rsp)\n"
    "movq %%rcx,  -24(%%rsp)\n"
    "movq %%rdx,  -32(%%rsp)\n"
    "movq %%rsi,  -40(%%rsp)\n"
    "movq %%rdi,  -48(%%rsp)\n"
    "movq %%rsp,  -56(%%rsp)\n"
    "movq %%rbp,  -64(%%rsp)\n"
    "movq %%r8,   -72(%%rsp)\n"
    "movq %%r9,   -80(%%rsp)\n"
    "movq %%r10,  -88(%%rsp)\n"
    "movq %%r11,  -96(%%rsp)\n"
    "movq %%r12, -104(%%rsp)\n"
    "movq %%r13, -112(%%rsp)\n"
    "movq %%r14, -120(%%rsp)\n"
    "movq %%r15, -128(%%rsp)\n"
    "sub $128, %%rsp\n"
    :
    :
    : "memory"
  );

  asm volatile(
    "lea 128(%%rsp), %%rbx\n"
  
    // argc
    "mov (%%rbx), %%rdi\n"

    // argv
    "lea 8(%%rbx), %%rsi\n"

    // envp
    "mov %%rdi, %%rax\n"
    "inc %%rax\n"
    "lea 8(%%rbx, %%rax, 8), %%rdx\n"

    "call *%[__evilcc_init]\n"
    :
    : [__evilcc_init] "r"(__evilcc_init)
    : "rax", "rdi", "rsi", "rdx", "rbx"
  );

  asm volatile(
    "add $128, %%rsp\n"
    "movq   -8(%%rsp),  %%rax\n"
    "movq  -16(%%rsp),  %%rbx\n"
    "movq  -24(%%rsp),  %%rcx\n"
    "movq  -32(%%rsp),  %%rdx\n"
    "movq  -40(%%rsp),  %%rsi\n"
    "movq  -48(%%rsp),  %%rdi\n"
    "movq  -56(%%rsp),  %%rsp\n"
    "movq  -64(%%rsp),  %%rbp\n"
    "movq  -72(%%rsp),  %%r8\n"
    "movq  -80(%%rsp),  %%r9\n"
    "movq  -88(%%rsp),  %%r10\n"
    "movq  -96(%%rsp),  %%r11\n"
    "movq -104(%%rsp),  %%r12\n"
    "movq -112(%%rsp),  %%r13\n"
    "movq -120(%%rsp),  %%r14\n"
    "movq -128(%%rsp),  %%r15\n"
    :
    :
    : "memory"
  );

  asm volatile ("jmp *%0" :: "r"(__EVILCC_REAL_ENTRY_SYMBOL));
}

#endif
