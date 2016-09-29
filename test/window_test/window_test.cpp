#include <gtest/gtest.h>
#include "..\..\src\geometric_entities\window.h"

using namespace I3D;



GTEST_API_ int main(int argc, char **argv) {
  printf("Running main() from gtest_main.cc\n");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
