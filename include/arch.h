#ifndef __EVILCC_ARCH_H
#define __EVILCC_ARCH_H

#if defined(__x86_64__)
#include "arch/x86_64/syscalls.h"
#include "arch/x86_64/misc.h"
#elif defined(__i386__)
#include "arch/i386/syscalls.h"
#include "arch/i386/misc.h"
#else
#error "unsupported architecture"
#endif

#endif
