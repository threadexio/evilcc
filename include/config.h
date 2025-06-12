#ifndef __EVILCC_CONFIG_H
#define __EVILCC_CONFIG_H

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
//
//  * __EVILCC_SET_LD_PRELOAD
//
//    Run the program using a specified `LD_PRELOAD` or the one provided by the
//    user.
//
//  * __EVILCC_FORCE_LD_PRELOAD
//
//    Run the program using a specified `LD_PRELOAD` without
//    respecting the choice of the user. (This takes precedence over
//    `__EVILCC_SET_LD_PRELOAD`)

///////////////////////////////////////////////////////////////////////////////
// Drop setuid/setgid bits. (only relevant if `__EVILCC_DISABLE_ASLR`)
///////////////////////////////////////////////////////////////////////////////

#if defined(__EVILCC_DISABLE_ASLR)

// The kernel silently discards `ADDR_NO_RANDOMIZE` from the personality of
// setuid/setgid binaries. This means we cannot just set `ADDR_NO_RANDOMIZE`
// and re-execute ourselves. We have to do a little dance to somehow re-execute
// without the setuid/setgid bits. Currently there are 2 supported ways to do
// this.
//
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
//        `-D__EVILCC_IS_SETUID`
//
//     * __EVILCC_IS_SETUID
//
//       This instructs the code to re-set the setuid bit on startup. If you
//       plan to have this binary as setuid, then define this.
//
//     * __EVILCC_IS_SETGID
//
//         `-D__EVILCC_IS_SETGID`
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
  #error "__EVILCC_DROP_SUGID_METHOD not specified, see the docs"
#elif __EVILCC_DROP_SUGID_METHOD == __EVILCC_DROP_SUGID_PRCTL
#elif __EVILCC_DROP_SUGID_METHOD == __EVILCC_DROP_SUGID_CHMOD
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

#endif
