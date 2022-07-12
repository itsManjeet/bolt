#include "plugin.hh"
using namespace bolt::plugins;
#include <iostream>

#define PLUGINS_LIST    \
  X(Unknown, "unknown") \
  X(Http, "http")

#include "http/http.hh"
#include "unknown/unknown.hh"

Plugin::List Plugin::plugins(YAML::Node const& node) {
  Plugin::List plugins;
  for (auto const& i : node["plugins"]) {
#define X(klass, id)                                    \
  if (id == i.first.as<std::string>()) {                \
    std::cerr << "loading plugin: " << id << std::endl; \
    plugins[id] = (std::make_shared<klass>(i.second));  \
  }
    PLUGINS_LIST
#undef X
  }

  return plugins;
}