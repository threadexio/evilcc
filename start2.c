#include <sys/personality.h>
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

always_inline static syscall0(__NR_getgid, gid_t, _getgid);
always_inline static syscall0(__NR_getuid, uid_t, _getuid);
always_inline static syscall1(__NR_personality, int, _personality, unsigned long, persona);
always_inline static syscall2(__NR_chmod, int, _chmod, const char*, pathname, mode_t, mode);
always_inline static syscall2(__NR_stat, int, _stat, const char*, pathname, struct stat* restrict, statbuf);
always_inline static syscall3(__NR_execve, int, _execve, const char*, pathname, const char**, argv, const char**, envp);
always_inline static syscall3(__NR_setresgid, int, _setresgid, gid_t, rgid, gid_t, egid, gid_t, sgid);
always_inline static syscall3(__NR_setresuid, int, _setresuid, uid_t, ruid, uid_t, euid, uid_t, suid);


#define getgid() _getgid()
#define getuid() _getuid()
#define personality(persona) _personality(persona)
#define chmod(pathname, mode) _chmod(pathname, mode)
#define stat(pathname, statbuf) _stat(pathname, statbuf)
#define execve(pathname, argv, envp) _execve(pathname, argv, envp)
#define setresgid(rgid, egid, sgid) _setresgid(rgid, egid, sgid)
#define setresuid(ruid, euid, suid) _setresuid(ruid, euid, suid)

#define chmodme(mode) chmod("/proc/self/exe", mode)
#define statme(statbuf) stat("/proc/self/exe", statbuf)
#define reexecve(argv, envp) execve("/proc/self/exe", argv, envp)

always_inline static void die() {
  asm volatile("int3");
}

// Promote to this real uid when running
#ifndef TARGET_UID
#define TARGET_UID 0
#endif

// Promote to this real gid when running
#ifndef TARGET_GID
#define TARGET_GID 0
#endif

// Is this binary supposed to be setuid?
#ifndef SETUID
#define SETUID 0
#endif

// Is this binary supposed to be setgid?
#ifndef SETGID
#define SETGID 0
#endif

static void evil_init(int argc, const char* argv[], const char* envp[]) {
  (void)argc;

  uid_t uid = getuid();
  if (uid != TARGET_UID)
    setresuid(TARGET_UID, TARGET_UID, TARGET_UID);

  gid_t gid = getgid();
  if (gid != TARGET_GID)
    setresgid(TARGET_GID, TARGET_GID, TARGET_GID);

  struct stat stat = {0};
  if (statme(&stat) != 0)
    die();

  // Re-set the SUID and SGID bits if needed.
  {
    #if SETUID == 1
      stat.st_mode |= S_ISUID;
    #endif

    #if SETGID == 1
      stat.st_mode |= S_ISGID;
    #endif

    if (chmodme(stat.st_mode) != 0)
      die();
  }

  int persona = personality(0xffffffff);
  if (persona == -1)
    die();

  if ((persona & ADDR_NO_RANDOMIZE) == 0) {
    if (personality(persona | ADDR_NO_RANDOMIZE) != 0)
      die();

    // Unset the problematic setuid and setgid bits temporarily.
    //
    // The kernel does not allow setuid/setgid binaries to run with
    // `ADDR_NO_RANDOMIZE` so we have to do this little dance where we disable
    // those bits, execute the program (now with `ADDR_NO_RANDOMIZE`) and then
    // re-set those bits. This works because we have promoted ourselves to
    // `TARGET_UID:TARGET_GID` before unsetting the setuid and setgid bits.
    if (chmodme(stat.st_mode & ~S_ISUID & ~S_ISGID) != 0)
      die();

    if (reexecve(argv, envp) != 0)
      die();
  }
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
