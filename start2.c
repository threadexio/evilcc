#include <sys/personality.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stddef.h>
#include <unistd.h>

#define naked __attribute__((naked))

#if 1
#define always_inline __attribute__((always_inline)) inline
#else
#define always_inline __attribute__((noinline))
#endif

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
always_inline static syscall2(__NR_stat, int, _stat, const char*, pathname, struct stat* restrict, statbuf);
always_inline static syscall3(__NR_execve, int, _execve, const char*, pathname, const char**, argv, const char**, envp);
always_inline static syscall3(__NR_setresgid, int, _setresgid, gid_t, rgid, gid_t, egid, gid_t, sgid);
always_inline static syscall3(__NR_setresuid, int, _setresuid, uid_t, ruid, uid_t, euid, uid_t, suid);
// always_inline static syscall3(__NR_write, ssize_t, _write, int, fd, const void*, buf, size_t, len);
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
// #define write(fd, buf, len) _write(fd, buf, len)
#define prctl(option, arg2, arg3, arg4, arg5) _prctl(option, arg2, arg3, arg4, arg5)

#define chmodme(mode) chmod("/proc/self/exe", mode)
#define statme(statbuf) stat("/proc/self/exe", statbuf)
#define reexecve(argv, envp) execve("/proc/self/exe", argv, envp)

always_inline static void die() {
  asm volatile("int3");
}


#define __EVILCC

///////////////////////////////////////////////////////////////////////////////
// General functionality toggles.
///////////////////////////////////////////////////////////////////////////////

// The following macros are used to enable functionality.
//
//  * __EVILCC_PROMOTE_UID
//
//    Promote the real (and saved) uid to the effective uid.
//
//  * __EVILCC_PROMOTE_GID
//
//    Promote the real (and saved) gid to the effective gid.
//
//  * __EVILCC_DISABLE_ASLR
//
//    Run the program with `ADDR_NO_RANDOMIZE`.

///////////////////////////////////////////////////////////////////////////////
// Drop setuid/setgid bits. (only relevant if `__EVILCC_DISABLE_ASLR`)
///////////////////////////////////////////////////////////////////////////////

#if defined(__EVILCC_DISABLE_ASLR)

// The kernel silently discards `ADDR_NO_RANDOMIZE` from the personality of
// setuid/setgid binaries. This means we cannot just set `ADDR_NO_RANDOMIZE`
// and re-execute ourselves. We have to do a little dance to somehow re-execute
// without the setuid/setgid bits. Currently there are 2 supported ways to do
// this.

// 1. By using `prctl`.
//
//        `-D__EVILCC_DROP_SUGID_METHOD=__EVILCC_DROP_SUGID_PRCTL`
//
//    Before re-executing we set the `NO_NEW_PRIVS` flag that tells the kernel
//    not to respect setuid/setgid bits when executing other programs (including
//    ourselves).
//
//    Downsides:
//
//     * Since the `NO_NEW_PRIVS` flag _cannot_ be unset, the actual program
//       must not depend on functionality that is inhibited by the presence of
//       this flag.
//
//    If your program does not need such functionality, you should really use
//    this method.
#define __EVILCC_DROP_SUGID_PRCTL 1

// 2. By using `chmod`.
//
//        `-D__EVILCC_DROP_SUGID_METHOD=__EVILCC_DROP_SUGID_CHMOD`
//
//    Before re-executing we use `chmod` to unset the setuid/setgid bits
//    from our binary, then we `execve` our own binary (which is now not
//    setuid/setgid) and then at startup we re-set the appropriate setuid/setgid
//    bits.
//
//    If using this method, the following macros take effect:
//
//        `-D__EVILCC_IS_SETUID=1`
//
//     * __EVILCC_IS_SETUID
//
//       This instructs the code to re-set the setuid bit on startup. If you
//       plan to have this binary as setuid, then define this.
//
//     * __EVILCC_IS_SETGID
//
//         `-D__EVILCC_IS_SETGID=1`
//
//       This instructs the code to re-set the setgid bit on startup. If you
//       plan to have this binary as setgid, then define this. 
//
//    Downsides:
//
//     * Between the `chmod` call that unsets the setuid/setgid bits and the
//       call that re-sets them there is a very small window of time where the
//       underlying binary is left without any setuid/setgid bits. This means
//       that it is possible for another instance to be started in that time
//       frame that does not work correctly because it doesn't have the correct
//       setuid/setgid bits.
#define __EVILCC_DROP_SUGID_CHMOD 2

#if !defined(__EVILCC_DROP_SUGID_METHOD)
  #error "__EVILCC_DROP_SUGID_METHOD not specified"

#elif __EVILCC_DROP_SUGID_METHOD == __EVILCC_DROP_SUGID_PRCTL

#elif __EVILCC_DROP_SUGID_METHOD == __EVILCC_DROP_SUGID_CHMOD
  #if !defined(__EVILCC_IS_SETUID)
    #error "__EVILCC_IS_SETUID not specified"
  #endif

  #if !defined(__EVILCC_IS_SETGID)
    #error "__EVILCC_IS_SETGID not specified"
  #endif

#else
#error "unknown __EVILCC_DROP_SUGID_METHOD, see the docs for available methods"
#endif

#endif

///////////////////////////////////////////////////////////////////////////////
// Advanced macros.
///////////////////////////////////////////////////////////////////////////////

// Name of the entry symbol for evilcc.
//
// This should be the name of the symbol the linker will set as the entrypoint.
// It must be the name of the symbol passed to the linker with `-Wl,-e"<name>"`.
#if !defined(__EVILCC_ENTRY_SYMBOL)
#define __EVILCC_ENTRY_SYMBOL __evilcc_entry
#endif

// Evilcc will jump to this symbol after it has finished its job.
//
// This should be the "real" entrypoint of the binary. The default is "_start"
// and works fine.
#if !defined(__EVILCC_REAL_ENTRY_SYMBOL)
#define __EVILCC_REAL_ENTRY_SYMBOL _start
#endif

///////////////////////////////////////////////////////////////////////////////

static void __evilcc_init(int argc, const char* argv[], const char* envp[]) {
  (void)argc;
  (void)argv;
  (void)envp;

#if defined(__EVILCC_PROMOTE_UID)
  {
    uid_t ruid = getuid();
    uid_t euid = geteuid();

    if (ruid != euid)
      if (setresuid(euid, euid, euid) != 0)
        die();
  }
#endif

#if defined(__EVILCC_PROMOTE_GID)
  {
    gid_t rgid = getgid();
    gid_t egid = getegid();

    if (rgid != egid)
      if (setresgid(egid, egid, egid) != 0)
        die();
  }
#endif

#if defined(__EVILCC_DISABLE_ASLR)
  {
    struct stat stat = {0};
    if (statme(&stat) != 0)
      die();

    // Re-set the SUID and SGID bits if needed.
    #if __EVILCC_DROP_SUGID_METHOD == __EVILCC_DROP_SUGID_CHMOD
      #if __EVILCC_IS_SETUID == 1
        stat.st_mode |= S_ISUID;
      #endif

      #if __EVILCC_IS_SETGID == 1
        stat.st_mode |= S_ISGID;
      #endif

      #if __EVILCC_IS_SETUID == 1 || __EVILCC_IS_SETGID == 1
        if (chmodme(stat.st_mode) != 0)
          die();
      #endif
    #endif

    int persona = personality(0xffffffff);
    if (persona == -1)
      die();

    if ((persona & ADDR_NO_RANDOMIZE) == 0) {
      if (personality(persona | ADDR_NO_RANDOMIZE) != 0)
        die();

      #if __EVILCC_DROP_SUGID_METHOD == __EVILCC_DROP_SUGID_PRCTL
        // Tell the kernel not to respect the setuid/setgid bits from now on.
        //
        // See the __EVILCC_DROP_SUGID_PRCTL docs for implications.
        if (prctl(PR_SET_NO_NEW_PRIVS, 1L, 0L, 0L, 0L) != 0)
          die();
      #elif __EVILCC_DROP_SUGID_METHOD == __EVILCC_DROP_SUGID_CHMOD
        // Unset the problematic setuid and setgid bits temporarily.
        //
        // See the `__EVILCC_DROP_SUGID_CHMOD` docs for more.
        if (chmodme(stat.st_mode & ~S_ISUID & ~S_ISGID) != 0)
          die();
      #endif

      if (reexecve(argv, envp) != 0)
        die();
    }
  }
#endif
}

extern void __EVILCC_REAL_ENTRY_SYMBOL(void);

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
