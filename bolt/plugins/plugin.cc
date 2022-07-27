#include "plugin.hh"
using namespace bolt::plugins;
#include <iostream>

#define PLUGINS_LIST    \
  X(Unknown, "unknown") \
  X(Http, "http")       \
  X(Shell, "shell")

#include "../Debug.hh"
#include "http/http.hh"
#include "shell/shell.hh"
#include "unknown/unknown.hh"

Plugin::List Plugin::plugins(YAML::Node const& node) {
  Plugin::List plugins;
  for (auto const& i : node["plugins"]) {
#define X(klass, id)                                   \
  if (id == i.first.as<std::string>()) {               \
    LOG("PLUGIN", "LOADING PLUGIN " << id);            \
    plugins[id] = (std::make_shared<klass>(i.second)); \
  } else

    PLUGINS_LIST
#undef X
    {
      LOG("PLUGIN", "INVALID PLUGIN " << i.first.as<std::string>());
    }
  }

  return plugins;
}