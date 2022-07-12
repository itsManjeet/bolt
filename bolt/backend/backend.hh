#ifndef BOLT_BACKEND_HH
#define BOLT_BACKEND_HH

#include <yaml-cpp/yaml.h>

namespace bolt {
class Bolt;
}

namespace bolt::backend {
class Backend {
 public:
  using ptr = std::shared_ptr<Backend>;

 protected:
  YAML::Node mNode;
  std::string mError;

 public:
  Backend(YAML::Node const& node) : mNode(node) {}

  virtual bool start(Bolt* bolt) = 0;

  std::string const& error() const { return mError; }

  static Backend::ptr create(YAML::Node const& node);
};
}  // namespace bolt::backend

#endif