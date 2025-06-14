#ifndef __EVILCC_ARCH_H
#define __EVILCC_ARCH_H

#if defined(__x86_64__)
#include "arch/x86_64/entry.h"
#include "arch/x86_64/misc.h"
#include "arch/x86_64/syscalls.h"
#elif defined(__i386__)
#include "arch/i386/entry.h"
#include "arch/i386/misc.h"
#include "arch/i386/syscalls.h"
#else
#error "unsupported architecture"
#endif

#endif
