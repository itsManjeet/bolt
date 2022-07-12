#include "logic.hh"
using namespace bolt::logics;

#define LOGICS_LIST X(Classifier, "classifier")
#include <iostream>

#include "classifier/classifier.hh"

Logic::List Logic::logics(YAML::Node const& node) {
  Logic::List logics;
  for (auto const& logic_node : node["logics"]) {
#define X(klass, id)                                   \
  if (logic_node.first.as<std::string>() == id)        \
    std::cout << "loading logic: " << id << std::endl; \
  logics.push_back(std::make_shared<klass>(logic_node.second));
    LOGICS_LIST
#undef X
  }

  return logics;
}