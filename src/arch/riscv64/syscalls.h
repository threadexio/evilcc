#ifndef __EVILCC_ARCH_RISCV64_SYSCALLS_H
#define __EVILCC_ARCH_RISCV64_SYSCALLS_H

#define syscall0(num, ret, name)                                              \
  ret name(void) {                                                            \
    register long a7 __asm__("a7") = (num);                                   \
    register long a0 __asm__("a0") = 0;                                       \
                                                                              \
    asm volatile(                                                             \
      "ecall"                                                                 \
      : "+r"(a0)                                                              \
      : "r"(a7), "r"(a0)                                                      \
      : "memory"                                                              \
    );                                                                        \
                                                                              \
    return (ret)a0;                                                           \
  }

#define syscall1(num, ret, name, arg1_t, arg1)                                \
  ret name(arg1_t arg1) {                                                     \
    register long a7 __asm__("a7") = (num);                                   \
    register long a0 __asm__("a0") = (long)(arg1);                            \
                                                                              \
    asm volatile(                                                             \
      "ecall"                                                                 \
      : "+r"(a0)                                                              \
      : "r"(a7), "r"(a0)                                                      \
      : "memory"                                                              \
    );                                                                        \
                                                                              \
    return (ret)a0;                                                           \
  }

#define syscall2(num, ret, name, arg1_t, arg1, arg2_t, arg2)                  \
  ret name(arg1_t arg1, arg2_t arg2) {                                        \
    register long a7 __asm__("a7") = (num);                                   \
    register long a0 __asm__("a0") = (long)(arg1);                            \
    register long a1 __asm__("a1") = (long)(arg2);                            \
                                                                              \
    asm volatile(                                                             \
      "ecall"                                                                 \
      : "+r"(a0)                                                              \
      : "r"(a7), "r"(a0), "r"(a1)                                             \
      : "memory"                                                              \
    );                                                                        \
                                                                              \
    return (ret)a0;                                                           \
  }


#define syscall3(num, ret, name, arg1_t, arg1, arg2_t, arg2, arg3_t, arg3)    \
  ret name(arg1_t arg1, arg2_t arg2, arg3_t arg3) {                           \
    register long a7 __asm__("a7") = (num);                                   \
    register long a0 __asm__("a0") = (long)(arg1);                            \
    register long a1 __asm__("a1") = (long)(arg2);                            \
    register long a2 __asm__("a2") = (long)(arg3);                            \
                                                                              \
    asm volatile(                                                             \
      "ecall"                                                                 \
      : "+r"(a0)                                                              \
      : "r"(a7), "r"(a0), "r"(a1), "r"(a2)                                    \
      : "memory"                                                              \
    );                                                                        \
                                                                              \
    return (ret)a0;                                                           \
  }

#define syscall4(num, ret, name, arg1_t, arg1, arg2_t, arg2, arg3_t, arg3, arg4_t, arg4) \
  ret name(arg1_t arg1, arg2_t arg2, arg3_t arg3, arg4_t arg4) {                         \
    register long a7 __asm__("a7") = (num);                                              \
    register long a0 __asm__("a0") = (long)(arg1);                                       \
    register long a1 __asm__("a1") = (long)(arg2);                                       \
    register long a2 __asm__("a2") = (long)(arg3);                                       \
    register long a3 __asm__("a3") = (long)(arg4);                                       \
                                                                                         \
    asm volatile(                                                                        \
      "ecall"                                                                            \
      : "+r"(a0)                                                                         \
      : "r"(a7), "r"(a0), "r"(a1), "r"(a2), "r"(a3)                                      \
      : "memory"                                                                         \
    );                                                                                   \
                                                                                         \
    return (ret)a0;                                                                      \
  }

#define syscall5(num, ret, name, arg1_t, arg1, arg2_t, arg2, arg3_t, arg3, arg4_t, arg4, arg5_t, arg5) \
  ret name(arg1_t arg1, arg2_t arg2, arg3_t arg3, arg4_t arg4, arg5_t arg5) {                          \
    register long a7 __asm__("a7") = (num);                                                            \
    register long a0 __asm__("a0") = (long)(arg1);                                                     \
    register long a1 __asm__("a1") = (long)(arg2);                                                     \
    register long a2 __asm__("a2") = (long)(arg3);                                                     \
    register long a3 __asm__("a3") = (long)(arg4);                                                     \
    register long a4 __asm__("a4") = (long)(arg5);                                                     \
                                                                                                       \
    asm volatile(                                                                                      \
      "ecall"                                                                                          \
      : "+r"(a0)                                                                                       \
      : "r"(a7), "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4)                                           \
      : "memory"                                                                                       \
    );                                                                                                 \
                                                                                                       \
    return (ret)a0;                                                                                    \
  }

#endif
