#include <stdio.h>

__attribute__((constructor))
static int evil_init(void) {
  printf("[libevil]\n");
  printf("[libevil] loaded!\n");
  printf("[libevil]\n");
  return 0;
}
