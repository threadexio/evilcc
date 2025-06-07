#include "config.h"
#include "init.h"

#if defined(__x86_64__)
#include "arch/x86_64/syscalls.h"
#include "arch/x86_64/misc.h"
#elif defined(__i386__)
#include "arch/i386/syscalls.h"
#include "arch/i386/misc.h"
#else
#error "unsupported architecture"
#endif

#define chmodme(mode) chmod("/proc/self/exe", mode)
#define statme(statbuf) stat("/proc/self/exe", statbuf)
#define reexecve(argv, envp) execve("/proc/self/exe", argv, envp)

void __evilcc_init(int argc, const char* argv[], const char* envp[]) {
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
