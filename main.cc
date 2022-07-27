#include <iostream>

#include "bolt/bolt.hh"
int main(int argc, char** argv) {
  if (argc == 1) {
    std::cout << "Usage: " << argv[0] << " [config.yml]" << std::endl;
    return 1;
  }
  std::cout << "starting bolt server" << std::endl;
  auto bolt = bolt::Bolt::create(argv[1]);
  if (!bolt->start()) {
    std::cerr << "bolt: " << bolt->error() << std::endl;
    return 1;
  }
  return 0;
}