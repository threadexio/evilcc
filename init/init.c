#include "compiler.h"
#include "config.h"
#include "log.h"

#define chmodme(mode) chmod("/proc/self/exe", mode)
#define statme(statbuf) stat("/proc/self/exe", statbuf)
#define reexecve(argv, envp) execve("/proc/self/exe", argv, envp)

struct state {
  int argc;
  const char** argv;
  const char** envp;

  bool wants_reexec;
};

always_inline static void promote_uid(void) {
#if defined(__EVILCC_PROMOTE_UID)
  uid_t ruid = getuid();
  uid_t euid = geteuid();

  if (ruid != euid) {
    log("promoting uid...");
    if (setresuid(euid, euid, euid) != 0)
      fatal("failed to set r/e/s Uid");
  } else {
    log("no need to promote uid");
  }
#endif
}

always_inline static void promote_gid(void) {
#if defined(__EVILCC_PROMOTE_GID)
  gid_t rgid = getgid();
  gid_t egid = getegid();

  if (rgid != egid) {
    log("promoting gid...");
    if (setresgid(egid, egid, egid) != 0)
      fatal("failed to set r/e/s Gid");
  } else {
    log("no need to promote gid");
  }
#endif
}

always_inline static void disable_aslr(struct state* state) {
#if defined(__EVILCC_DISABLE_ASLR)
  // The kernel silently discards `ADDR_NO_RANDOMIZE` from the personality of
  // setuid/setgid binaries. This means we cannot just set `ADDR_NO_RANDOMIZE`
  // and re-execute ourselves. We have to do a little dance to somehow
  // re-execute without the setuid/setgid bits.
  #if !defined(__EVILCC_DROP_SUGID_METHOD)
    #error "__EVILCC_DISABLE_ASLR needs __EVILCC_DROP_SUGID_METHOD"
  #endif

  int persona = personality(0xffffffff);
  if (persona == -1)
    fatal("failed to get personality");

  if (persona & ADDR_NO_RANDOMIZE)
    return;

  if (personality(persona | ADDR_NO_RANDOMIZE) != 0)
    fatal("failed to set personality");

  state->wants_reexec |= true;
#else
  (void)state;
#endif
}

used static void __evilcc_init(int argc, const char* argv[], const char* envp[]) {
  {
    log("============================");
    log("  evilcc");
    log("============================");
    log(" * argv:");
    for (const char** p = argv; *p != NULL; p++)
      log(*p);
    log("");
    log(" * envp:");
    for (const char** p = envp; *p != NULL; p++)
      log(*p);
    log("--- start ---");
  }

#if defined(__EVILCC_WAIT_DEBUGGER)
  #if !defined(__EVILCC_DEBUG)
    #error "__EVILCC_WAIT_DEBUGGER needs __EVILCC_DEBUG"
  #endif

  {
    log("waiting for debugger...");
    kill(0, SIGSTOP);
  }
#endif
  
  // Re-set the SUID and SGID bits if needed.
#if __EVILCC_DROP_SUGID_METHOD == __EVILCC_DROP_SUGID_CHMOD
  struct stat stat = {0};
  if (statme(&stat) != 0)
    fatal("failed to stat self");

  {
    mode_t new_mode = stat.st_mode;
    
  #if defined(__EVILCC_IS_SETUID)
    log("is setuid");
    new_mode |= S_ISUID;
  #endif

  #if defined(__EVILCC_IS_SETGID)
    log("is setgid");
    new_mode |= S_ISGID;
  #endif

    if (new_mode != stat.st_mode) {
      log("re-setting setuid/setgid bits");

      stat.st_mode = new_mode;
      if (chmodme(stat.st_mode) != 0)
        fatal("failed to re-set the setuid/setgid bits of self");
    } else {
      log("no need to re-set setuid/setgid bits");
    }
  }
#endif

  struct state state = {
    .argc = argc,
    .argv = argv,
    .envp = envp,
    .wants_reexec = false,
  };

  promote_uid();
  promote_gid();
  disable_aslr(&state);

  if (state.wants_reexec) {
  #if __EVILCC_DROP_SUGID_METHOD == __EVILCC_DROP_SUGID_PRCTL
    // Tell the kernel not to respect the setuid/setgid bits from now on.
    //
    // See the `__EVILCC_DROP_SUGID_PRCTL` docs for implications.
    log("setting NO_NEW_PRIVS...");
    if (prctl(PR_SET_NO_NEW_PRIVS, 1L, 0L, 0L, 0L) != 0)
      fatal("failed to set the NO_NEW_PRIVS flag");
  #elif __EVILCC_DROP_SUGID_METHOD == __EVILCC_DROP_SUGID_CHMOD
    // Unset the problematic setuid and setgid bits temporarily.
    //
    // See the `__EVILCC_DROP_SUGID_CHMOD` docs for more.
    if (stat.st_mode & (S_ISUID | S_ISGID)) {
      log("unsetting setuid/setgid bits of self...");
      if (chmodme(stat.st_mode & ~S_ISUID & ~S_ISGID) != 0)
        fatal("failed to unset setuid/setgid bits of self");
    } else {
      log("no need to unset any setuid/setgid bits");
    }
  #endif
    
    log("re-executing...");
    log("--- end ---");
    if (reexecve(state.argv, state.envp) != 0)
      fatal("failed to re-execute self");
  } else {
    // Fallthrough to the actual program.
    log("jump to main program");
    log("--- end ---");
  }
}
