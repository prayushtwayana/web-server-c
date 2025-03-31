#include "common.h"
#include <stdio.h>

void error(const char *msg) {
  perror(msg);
  exit(1);
}
