#ifndef __EVILCC_ARCH_I386_ENTRY_H
#define __EVILCC_ARCH_I386_ENTRY_H

#include "compiler.h"
#include "config.h"

used static void __evilcc_init(int argc, const char* argv[], const char* envp[]);

asm (
  ".text\n"
  ".globl " stringify(__EVILCC_ENTRY_SYMBOL) "\n"
  stringify(__EVILCC_ENTRY_SYMBOL) ":\n"
  "pusha\n"
  "pushfl\n"

  "lea 36(%esp), %ebx\n"
  "sub $12, %esp\n"
  "mov 0(%ebx), %eax\n"
  "mov %eax, 0(%esp)\n"          // argc
  "lea 8(%ebx, %eax, 4), %eax\n"
  "mov %eax, 8(%esp)\n"          // envp
  "lea 4(%ebx), %eax\n"
  "mov %eax, 4(%esp)\n"          // argv
  "call __evilcc_init\n"
  "add $12, %esp\n"

  "popfl\n"
  "popa\n"

  "jmp " stringify(__EVILCC_REAL_ENTRY_SYMBOL) "\n"
);

#endif
