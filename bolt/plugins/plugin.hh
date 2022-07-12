#ifndef BOLT_PLUGIN_HH
#define BOLT_PLUGIN_HH

#include <yaml-cpp/yaml.h>
namespace bolt::plugins {
class Plugin {
 public:
  using ptr = std::shared_ptr<Plugin>;
  using List = std::map<std::string, Plugin::ptr>;

 protected:
  std::string mError;
  YAML::Node mNode;

 public:
  Plugin(YAML::Node const& node) : mNode(node) {}

  virtual std::string respond(std::string intension, std::string mesg) = 0;

  static List plugins(YAML::Node const& node);
};
}  // namespace bolt::plugin

#endif