#ifndef BOLT_HH
#define BOLT_HH

#include "backend/backend.hh"
#include "logics/logic.hh"
#include "plugins/plugin.hh"

namespace bolt {
/**
 * @brief Central class for handling bolt API
 */
class Bolt {
 public:
  using ptr = std::shared_ptr<Bolt>;

 private:
  logics::Logic::List mLogics;
  plugins::Plugin::List mPlugins;
  backend::Backend::ptr mBackend;
  std::string mId;
  std::string mError;

 public:
  Bolt(YAML::Node const& node);

  static Bolt::ptr create(std::string filepath);

  std::string respond(std::string intention, std::string input);

  std::tuple<float, std::string> predict(std::string input);

  std::string const& error() const { return mError; }

  bool train(std::string logicId, std::string modelPath);
  
  bool start();
};
}  // namespace bolt

#endif