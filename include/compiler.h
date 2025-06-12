#ifndef __EVILCC_COMPILER_H
#define __EVILCC_COMPILER_H

#define stringify(x) stringify_(x)
#define stringify_(x) #x

#define naked         __attribute__((naked))
#define used          __attribute__((used))
#define always_inline __attribute__((always_inline)) inline

#endif
