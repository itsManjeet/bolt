#ifndef BOLT_PLUGINS_HH
#define BOLT_PLUGINS_HH

#include "../plugin.hh"
namespace bolt::plugins {
class Http : public Plugin {
 public:
  Http(YAML::Node const& node) : Plugin(node) {}

  std::string request(std::string url, std::string data);

  std::string respond(std::string intension, std::string mesg);
};
}  // namespace bolt::plugins

#endif