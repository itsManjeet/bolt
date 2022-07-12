#include <dpp/dpp.h>

#include "../../bolt.hh"
#include "../backend.hh"

namespace bolt::backend {
class Discord : public Backend {
 private:
  std::shared_ptr<dpp::cluster> mBackend;

 public:
  Discord(YAML::Node const& node) : Backend(node) {}

  bool start(Bolt* bolt);
};
}  // namespace bolt::backend