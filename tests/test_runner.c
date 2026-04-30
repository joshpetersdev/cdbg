#include "cdbg/test.h"

TEST(test_environment) {
  REQUIRE(1 == 1);
}

TEST(test_breakpoints) {
  REQUIRE(2 + 2 == 4);
}

TEST(test_elf_parser) {
  REQUIRE(1);
}

int main(void) {
  test_environment();
  test_breakpoints();
  test_elf_parser();

  printf("All tests passed!\n");
  return 0;
}
