#ifndef __EVILCC_ENV_H
#define __EVILCC_ENV_H

#include "polyfill.h"

typedef struct env {
  const char** envp;
  size_t len;
  size_t capacity;
} env_t;

#define env_new(cap) ({                                                       \
    size_t _capacity = (cap);                                                 \
                                                                              \
    (env_t) {                                                                 \
      .envp = alloca_zeroed(sizeof(char*) * (_capacity + 1)),                 \
      .capacity = _capacity,                                                  \
      .len = 0,                                                               \
    };                                                                        \
  })

always_inline static ssize_t env_find(env_t* restrict self, const char* var);
always_inline static const char* env_get(env_t* restrict self, size_t i);
always_inline static void env_set(env_t* restrict self, size_t i, const char* value);
always_inline static bool _env_insert(env_t* restrict self, bool force, const char* var, const char* varvalue);
#define env_insert(self, force, var, value) _env_insert(self, force, var, var "=" value)
always_inline static const char* env_remove(env_t* restrict self, const char* var);
always_inline static void env_push(env_t* restrict self, const char* value);
always_inline static void env_append(env_t* restrict self, const char** other, size_t len);

always_inline static ssize_t env_find(env_t* restrict self, const char* var) {
  size_t var_len = strlen(var);
  
  for (size_t i = 0; i < self->len; i++)
    if (strneq(self->envp[i], var, var_len))
      return i;

  return -1;
}

always_inline static const char* env_get(env_t* restrict self, size_t i) {
  if (i >= self->len)
    die();

  return self->envp[i];
}

always_inline static void env_set(env_t* restrict self, size_t i, const char* value) {
  if (i >= self->len)
    die();

  self->envp[i] = value;
}

always_inline static bool _env_insert(env_t* restrict self, bool force, const char* var, const char* varvalue) {
  ssize_t i = env_find(self, var);

  if (i == -1) {
    env_push(self, varvalue);
    return true;
  }

  if (force && !streq(self->envp[i], varvalue)) {
    env_set(self, i, varvalue);
    return true;
  }

  return false;
}

always_inline static const char* env_remove(env_t* restrict self, const char* var) {
  ssize_t i = env_find(self, var);
  if (i == -1)
    return NULL;

  const char* old = self->envp[i];

  self->envp[i] = self->envp[self->len];
  self->envp[self->len] = NULL;
  self->len -= 1;
  return old;
}

always_inline static void env_push(env_t* restrict self, const char* value) {
  if (self->len > self->capacity)
    die();

  self->envp[self->len++] = value;
  self->envp[self->len] = NULL;
}

always_inline static void env_append(env_t* restrict self, const char** other, size_t len) {
  if (len == 0)
    return;

  size_t new_len = self->len + len;
  if (new_len > self->capacity)
    die();

  for (size_t i = 0; i < len; i++)
    self->envp[self->len + i] = other[i];

  self->envp[new_len] = NULL;
  self->len = new_len;
}

#endif
