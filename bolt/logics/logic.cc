#include "logic.hh"
using namespace bolt::logics;

#define LOGICS_LIST           \
  X(Classifier, "classifier") \
  X(Distance, "distance")

#include <iostream>

#include "../Debug.hh"
#include "classifier/classifier.hh"
#include "distance/distance.hh"

Logic::List Logic::logics(YAML::Node const& node) {
  Logic::List logics;
  for (auto const& logic_node : node["logics"]) {
#define X(klass, id)                                                \
  if (logic_node.first.as<std::string>() == id) {                   \
    LOG("LOGIC", "LOADING " << id);                                 \
    logics.push_back({logic_node.first.as<std::string>(),           \
                      std::make_shared<klass>(logic_node.second)}); \
  } else

    LOGICS_LIST
#undef X
    {
      LOG("LOGIC", "INVALID LOGIC " << logic_node.first.as<std::string>());
    }
  }

  return logics;
}