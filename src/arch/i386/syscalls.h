#ifndef __EVILCC_ARCH_I386_SYSCALLS_H
#define __EVILCC_ARCH_I386_SYSCALLS_H

#define syscall0(num, ret, name)                                              \
  ret name(void) {                                                            \
    ret result;                                                               \
                                                                              \
    asm volatile(                                                             \
      "int $0x80"                                                             \
      : "=a"(result)                                                          \
      : "a"(num)                                                              \
      : "memory"                                                              \
    );                                                                        \
                                                                              \
    return result;                                                            \
  }

#define syscall1(num, ret, name, arg1_t, arg1)                                \
  ret name(arg1_t arg1) {                                                     \
    ret result;                                                               \
                                                                              \
    asm volatile(                                                             \
      "int $0x80"                                                             \
      : "=a"(result)                                                          \
      : "a"(num), "b"(arg1)                                                   \
      : "memory"                                                              \
    );                                                                        \
                                                                              \
    return result;                                                            \
  }

#define syscall2(num, ret, name, arg1_t, arg1, arg2_t, arg2)\
  ret name(arg1_t arg1, arg2_t arg2) {                                        \
    ret result;                                                               \
                                                                              \
    asm volatile(                                                             \
      "int $0x80"                                                             \
      : "=a"(result)                                                          \
      : "a"(num), "b"(arg1), "c"(arg2)                                        \
      : "memory"                                                              \
    );                                                                        \
                                                                              \
    return result;                                                            \
  }

#define syscall3(num, ret, name, arg1_t, arg1, arg2_t, arg2, arg3_t, arg3)    \
  ret name(arg1_t arg1, arg2_t arg2, arg3_t arg3) {                           \
    ret result;                                                               \
                                                                              \
    asm volatile(                                                             \
      "int $0x80"                                                             \
      : "=a"(result)                                                          \
      : "a"(num), "b"(arg1), "c"(arg2), "d"(arg3)                             \
      : "memory"                                                              \
    );                                                                        \
                                                                              \
    return result;                                                            \
  }

#define syscall4(num, ret, name, arg1_t, arg1, arg2_t, arg2, arg3_t, arg3, arg4_t, arg4) \
  ret name(arg1_t arg1, arg2_t arg2, arg3_t arg3, arg4_t arg4) {                         \
    ret result;                                                                          \
                                                                                         \
    asm volatile(                                                                        \
      "int $0x80"                                                                        \
      : "=a"(result)                                                                     \
      : "a"(num), "b"(arg1), "c"(arg2), "d"(arg3), "S"(arg4)                             \
      : "memory"                                                                         \
    );                                                                                   \
                                                                                         \
    return result;                                                                       \
  }

#define syscall5(num, ret, name, arg1_t, arg1, arg2_t, arg2, arg3_t, arg3, arg4_t, arg4, arg5_t, arg5) \
  ret name(arg1_t arg1, arg2_t arg2, arg3_t arg3, arg4_t arg4, arg5_t arg5) {                          \
    ret result;                                                                                        \
                                                                                                       \
    asm volatile(                                                                                      \
      "int $0x80"                                                                                      \
      : "=a"(result)                                                                                   \
      : "a"(num), "b"(arg1), "c"(arg2), "d"(arg3), "S"(arg4), "D"(arg5)                                \
      : "memory"                                                                                       \
    );                                                                                                 \
                                                                                                       \
    return result;                                                                                     \
  }

#endif
