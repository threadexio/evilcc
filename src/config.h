#ifndef __EVILCC_CONFIG_H
#define __EVILCC_CONFIG_H

///////////////////////////////////////////////////////////////////////////////
// General functionality.
///////////////////////////////////////////////////////////////////////////////

// The following macros are used to customize functionality.
//
//  * __EVILCC_SETUID=<uid>
//
//    Set the real, effective and saved uid to <uid>. If <uid> is < 0, then
//    the real and saved uids are set to the effective uid. This can be used in
//    combination with setuid executables to fully promote the running user.
//
//  * __EVILCC_SETGID=<gid>
//
//    Set the real, effective and saved gid to <gid>. If <gid> is < 0, then
//    the real and saved gids are set to the effective gid. This can be used in
//    combination with setgid executables to fully promote the running user.
//
//  * __EVILCC_PERSONALITY=<persona>
//
//    Add <persona> to the personality of the program.
//
//  * __EVILCC_PERSONALITY_MASK=<persona>
//
//    Remove <persona> from the personality of the program. Note:
//    `__EVILCC_PERSONALITY` takes priority over this.

///////////////////////////////////////////////////////////////////////////////
// Drop setuid/setgid bits.
///////////////////////////////////////////////////////////////////////////////

// 1. By using `prctl`.
//
//        `-D__EVILCC_DROP_SUGID=__EVILCC_DROP_SUGID_PRCTL`
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
#define __EVILCC_DROP_SUGID_PRCTL 5

// 2. By using `chmod`.
//
//        `-D__EVILCC_DROP_SUGID=__EVILCC_DROP_SUGID_CHMOD`
//
//    Before re-executing we use `chmod` to unset the setuid/setgid bits
//    from our binary, then we `execve` our own binary (which is now not
//    setuid/setgid) and then at startup we re-set the appropriate setuid/setgid
//    bits.
//
//    If using this method, the following macros take effect:
//
//     * __EVILCC_IS_SETUID
//
//        `-D__EVILCC_IS_SETUID`
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
#define __EVILCC_DROP_SUGID_CHMOD 6

#if !defined(__EVILCC_DROP_SUGID)
#elif __EVILCC_DROP_SUGID == __EVILCC_DROP_SUGID_PRCTL
#elif __EVILCC_DROP_SUGID == __EVILCC_DROP_SUGID_CHMOD
#else
  #error "unknown __EVILCC_DROP_SUGID, see the docs"
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
