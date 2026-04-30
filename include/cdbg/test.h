#ifndef CDBG_TEST_H
#define CDBG_TEST_H

#include <stdio.h>
#include <stdlib.h>

#define REQUIRE(expr) \
  do { \
    if (!(expr)) { \
      printf("[FAIL] %s:%d: %s\n", __FILE__, __LINE__, #expr); \
      exit(1); \
    } \
  } while (0)

#define TEST(name) void name(void)

#endif
