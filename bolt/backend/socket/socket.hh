#ifndef BOLT_BACKEND_SOCKET_HH
#define BOLT_BACKEND_SOCKET_HH

#include <systemd/sd-daemon.h>

#include "../backend.hh"

namespace bolt::backend {
class Socket : public Backend {
 public:
  Socket(YAML::Node const& node) : Backend(node) {}
  bool start(Bolt* bolt);
};
}  // namespace bolt::backend

#endif