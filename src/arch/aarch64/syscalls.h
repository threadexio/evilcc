#ifndef __EVILCC_ARCH_AARCH64_SYSCALLS_H
#define __EVILCC_ARCH_AARCH64_SYSCALLS_H

#define syscall0(num, ret, name)                                              \
  ret name(void) {                                                            \
    register long x8 __asm__("x8") = (num);                                   \
    register long x0 __asm__("x0") = 0;                                       \
                                                                              \
    asm volatile(                                                             \
      "svc #0"                                                                \
      : "=r"(x0)                                                              \
      : "r"(x8), "0"(x0)                                                      \
      : "memory"                                                              \
    );                                                                        \
                                                                              \
    return (ret)x0;                                                           \
  }

#define syscall1(num, ret, name, arg1_t, arg1)                                \
  ret name(arg1_t arg1) {                                                     \
    register long x8 __asm__("x8") = (num);                                   \
    register long x0 __asm__("x0") = (long)(arg1);                            \
                                                                              \
    asm volatile(                                                             \
      "svc #0"                                                                \
      : "=r"(x0)                                                              \
      : "r"(x8), "0"(x0)                                                      \
      : "memory"                                                              \
    );                                                                        \
                                                                              \
    return (ret)x0;                                                           \
  }

#define syscall2(num, ret, name, arg1_t, arg1, arg2_t, arg2)                  \
  ret name(arg1_t arg1, arg2_t arg2) {                                        \
    register long x8 __asm__("x8") = (num);                                   \
    register long x0 __asm__("x0") = (long)(arg1);                            \
    register long x1 __asm__("x1") = (long)(arg2);                            \
                                                                              \
    asm volatile(                                                             \
      "svc #0"                                                                \
      : "=r"(x0)                                                              \
      : "r"(x8), "0"(x0), "r"(x1)                                             \
      : "memory"                                                              \
    );                                                                        \
                                                                              \
    return (ret)x0;                                                           \
  }


#define syscall3(num, ret, name, arg1_t, arg1, arg2_t, arg2, arg3_t, arg3)    \
  ret name(arg1_t arg1, arg2_t arg2, arg3_t arg3) {                           \
    register long x8 __asm__("x8") = (num);                                   \
    register long x0 __asm__("x0") = (long)(arg1);                            \
    register long x1 __asm__("x1") = (long)(arg2);                            \
    register long x2 __asm__("x2") = (long)(arg3);                            \
                                                                              \
    asm volatile(                                                             \
      "svc #0"                                                                \
      : "=r"(x0)                                                              \
      : "r"(x8), "0"(x0), "r"(x1), "r"(x2)                                    \
      : "memory"                                                              \
    );                                                                        \
                                                                              \
    return (ret)x0;                                                           \
  }

#define syscall4(num, ret, name, arg1_t, arg1, arg2_t, arg2, arg3_t, arg3, arg4_t, arg4) \
  ret name(arg1_t arg1, arg2_t arg2, arg3_t arg3, arg4_t arg4) {                         \
    register long x8 __asm__("x8") = (num);                                              \
    register long x0 __asm__("x0") = (long)(arg1);                                       \
    register long x1 __asm__("x1") = (long)(arg2);                                       \
    register long x2 __asm__("x2") = (long)(arg3);                                       \
    register long x3 __asm__("x3") = (long)(arg4);                                       \
                                                                                         \
    asm volatile(                                                                        \
      "svc #0"                                                                           \
      : "=r"(x0)                                                                         \
      : "r"(x8), "0"(x0), "r"(x1), "r"(x2), "r"(x3)                                      \
      : "memory"                                                                         \
    );                                                                                   \
                                                                                         \
    return (ret)x0;                                                                      \
  }

#define syscall5(num, ret, name, arg1_t, arg1, arg2_t, arg2, arg3_t, arg3, arg4_t, arg4, arg5_t, arg5) \
  ret name(arg1_t arg1, arg2_t arg2, arg3_t arg3, arg4_t arg4, arg5_t arg5) {                          \
    register long x8 __asm__("x8") = (num);                                                            \
    register long x0 __asm__("x0") = (long)(arg1);                                                     \
    register long x1 __asm__("x1") = (long)(arg2);                                                     \
    register long x2 __asm__("x2") = (long)(arg3);                                                     \
    register long x3 __asm__("x3") = (long)(arg4);                                                     \
    register long x4 __asm__("x4") = (long)(arg5);                                                     \
                                                                                                       \
    asm volatile(                                                                                      \
      "svc #0"                                                                                         \
      : "=r"(x0)                                                                                       \
      : "r"(x8), "0"(x0), "r"(x1), "r"(x2), "r"(x3), "r"(x4)                                           \
      : "memory"                                                                                       \
    );                                                                                                 \
                                                                                                       \
    return (ret)x0;                                                                                    \
  }

#endif
