#include "backend.hh"
using namespace bolt::backend;

#include "discord/discord.hh"
#include "socket/socket.hh"

#define BACKEND_LIST    \
  X(Discord, "discord") \
  X(Socket, "socket")

Backend::ptr Backend::create(YAML::Node const& node) {
  auto backend_id = node["id"].as<std::string>();

#define X(klass, id) \
  if (id == backend_id) return std::make_shared<klass>(node);
  BACKEND_LIST
#undef X
  return nullptr;
}