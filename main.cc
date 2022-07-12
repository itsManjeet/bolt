#include <iostream>

#include "bolt/bolt.hh"
int main(int argc, char** argv) {
  std::cout << "starting bolt server" << std::endl;
  bolt::Bolt::create("bolt.yml")->start();
  
}