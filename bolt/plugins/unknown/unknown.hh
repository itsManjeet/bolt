#ifndef BOLT_PLUGINS_UNKNOWN_HH
#define BOLT_PLUGINS_UNKNOWN_HH

#include "../plugin.hh"
namespace bolt::plugins {
class Unknown : public Plugin {
 public:
  Unknown(YAML::Node const& node) : Plugin(node) {}

  std::string respond(std::string intension, std::string mesg);
};
}  // namespace bolt::plugins

#endif