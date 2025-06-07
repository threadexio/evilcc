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

// The following macros are used to control whether to promote the real (and
// saved) uid/gid to the effective uid/gid.
//
//  * __EVILCC_PROMOTE_UID
//
//    Promote the real uid to the effective uid.
//
//  * __EVILCC_PROMOTE_GID
//
//    Promote the real gid to the effective gid.
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

static void evil_init(int argc, const char* argv[], const char* envp[]) {
  (void)argc;

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

extern void _start(void);

naked void _start2(void) {
  asm volatile (
    // argc
    "mov (%%rsp), %%rdi\n"

    // argv
    "lea 8(%%rsp), %%rsi\n"

    // envp
    "mov %%rdi, %%rax\n"
    "inc %%rax\n"
    "lea 8(%%rsp, %%rax, 8), %%rdx\n"

    "call *%[evil_init]\n"
    :
    : [evil_init] "r"(evil_init)
    : "rax", "rdi", "rsi", "rdx"
  );

  asm volatile ("jmp *%[_start]" :: [_start] "r"(_start));
}
