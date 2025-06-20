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

always_inline static void do_setuid(struct state* state) {
#if defined(__EVILCC_SETUID)
  (void)state;

  uid_t target;

  #if __EVILCC_SETUID < 0
    target = geteuid();
    log("set uid to: (effective)");
  #else
    target = __EVILCC_SETUID;
    log("set uid to: " stringify(__EVILCC_SETUID));
  #endif

  if (setresuid(target, target, target) != 0)
    fatal("failed to set uid");
#else
  (void)state;
#endif
}

always_inline static void do_setgid(struct state* state) {
#if defined(__EVILCC_SETGID)
  (void)state;

  gid_t target;

  #if __EVILCC_SETGID < 0
    target = getegid();
    log("set gid to: (effective)");
  #else
    target = __EVILCC_SETGID;
    log("set gid to: " stringify(__EVILCC_SETGID));
  #endif

  if (setresgid(target, target, target) != 0)
    fatal("failed to set gid");
#else
  (void)state;
#endif
}

always_inline static void do_personality(struct state* state) {
#if defined(__EVILCC_PERSONALITY) || defined(__EVILCC_PERSONALITY_MASK)
  // The kernel silently drops some personality flags when executing
  // setuid/setgid binaries. For this reason, we have to do a little dance and
  // somehow get the kernel to re-execute without the setuid/setgid bits.
  #if defined(__EVILCC_PERSONALITY) && !defined(__EVILCC_DROP_SUGID)
    #error "__EVILCC_PERSONALITY needs __EVILCC_DROP_SUGID"
  #endif

  int persona = personality(0xffffffff);
  if (persona == -1)
    fatal("failed to get personality");

  int desired = persona;

  #if defined(__EVILCC_PERSONALITY_MASK)
    // Disable all flags not allowed by the mask.
    desired &= ~(__EVILCC_PERSONALITY_MASK);
  #endif

  #if defined(__EVILCC_PERSONALITY)
    // Set any flags needed.
    desired |= (__EVILCC_PERSONALITY);
  #endif

  if (persona == desired)
    return;

  #if defined(__EVILCC_PERSONALITY_MASK)
    log("personality: -" stringify(__EVILCC_PERSONALITY_MASK));
  #endif
  #if defined(__EVILCC_PERSONALITY)
    log("personality: +" stringify(__EVILCC_PERSONALITY));
  #endif

  log("set personality");
  if (personality(desired) == -1)
    fatal("failed to set personality");

  state->wants_reexec |= true;
#else
  (void)state;
#endif
}

used static finish_token_t __evilcc_init(int argc, const char* argv[], const char* envp[]) {
#if defined(__EVILCC_WAIT_DEBUGGER)
  #if !defined(__EVILCC_DEBUG)
    #error "__EVILCC_WAIT_DEBUGGER needs __EVILCC_DEBUG"
  #endif
  {
    log("waiting for debugger...");
    kill(0, SIGSTOP);
  }
#endif

  log("--- start ---");
  
  // Re-set the SUID and SGID bits if needed.
#if __EVILCC_DROP_SUGID == __EVILCC_DROP_SUGID_CHMOD
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

  do_setuid(&state);
  do_setgid(&state);
  do_personality(&state);

  if (state.wants_reexec) {
  #if __EVILCC_DROP_SUGID == __EVILCC_DROP_SUGID_PRCTL
    // Tell the kernel not to respect the setuid/setgid bits from now on.
    //
    // See the `__EVILCC_DROP_SUGID_PRCTL` docs for implications.
    log("setting NO_NEW_PRIVS...");
    if (prctl(PR_SET_NO_NEW_PRIVS, 1L, 0L, 0L, 0L) != 0)
      fatal("failed to set the NO_NEW_PRIVS flag");
  #elif __EVILCC_DROP_SUGID == __EVILCC_DROP_SUGID_CHMOD
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

  evilcc_finish();
}
