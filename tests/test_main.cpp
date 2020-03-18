#include "gtest/gtest.h"
#include <iostream>
//#include "../inc/main.hpp"

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  std::cout << "hello";
  return RUN_ALL_TESTS();
}
