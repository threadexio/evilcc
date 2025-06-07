#ifndef __EVILCC_COMPILER_H
#define __EVILCC_COMPILER_H

#define stringify(x) stringify_(x)
#define stringify_(x) #x

#define naked __attribute__((naked))

#if 1
#define always_inline __attribute__((always_inline)) inline
#else
#define always_inline __attribute__((noinline))
#endif

#endif
