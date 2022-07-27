#include "socket.hh"
using namespace bolt::backend;

#include <sys/socket.h>
#include <sys/un.h>

#include "../../Debug.hh"
#include "../../bolt.hh"
#include "httplib.h"

const std::string RESPONSE_STRUCTURE = R"({
  "status": %s,
  "data": "%s"
})";

template <typename... Args>
std::string string_format(const std::string& format, Args... args) {
  int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) +
               1;  // Extra space for '\0'
  if (size_s <= 0) {
    throw std::runtime_error("Error during formatting.");
  }
  auto size = static_cast<size_t>(size_s);
  std::unique_ptr<char[]> buf(new char[size]);
  std::snprintf(buf.get(), size, format.c_str(), args...);
  return std::string(buf.get(),
                     buf.get() + size - 1);  // We don't want the '\0' inside
}

bool Socket::start(Bolt* bolt) {
  int fd, n;
  httplib::Server server;

  std::string socket_path = "/run/bolt.socket";
  if (mNode["path"]) socket_path = mNode["path"].as<std::string>();

  n = sd_listen_fds(0);
  if (n > 1) {
    mError = "too many file descriptors received";
    return false;
  } else if (n == 1) {
    fd = SD_LISTEN_FDS_START + 0;
  } else {
    union {
      struct sockaddr sa;
      struct sockaddr_un un;
    } sa;

    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
      mError = "socket() failed: " + std::string(strerror(errno));
      return false;
    }

    memset(&sa, 0, sizeof(sa));
    sa.un.sun_family = AF_UNIX;
    strncpy(sa.un.sun_path, socket_path.c_str(), sizeof(sa.un.sun_path));
    if (bind(fd, &sa.sa, sizeof(sa)) < 0) {
      mError = "bind() failed: " + std::string(strerror(errno));
      return false;
    }

    if (listen(fd, SOMAXCONN) < 0) {
      mError = "listen() failed: " + std::string(strerror(errno));
      return false;
    }
  }

  server.Get("/predict", [&bolt](httplib::Request const& request,
                                 httplib::Response& res) {
    bool status = false;
    std::string data = "invalid or no 'input' parameter provided";

    if (request.has_param("input")) {
      std::string input = request.get_param_value("input");
      auto [score, message] = bolt->predict(input);
      if (score >= 50.0f) {
        status = true;
      }
      data = message;
    }
    res.set_content(string_format(RESPONSE_STRUCTURE, status ? "true" : "false",
                                  data.c_str()),
                    "application/json");
  });

  server.Get("/train", [&bolt](httplib::Request const& request,
                               httplib::Response& response) {
    bool status = false;
    std::string mesg =
        "invalid parameters or no 'logic' and 'trainfile' parameter specified";

    if (request.has_param("logic") && request.has_param("trainfile")) {
      std::string logic = request.get_param_value("logic");
      std::string trainfile = request.get_param_value("trainfile");

      if (bolt->train(logic, trainfile)) {
        status = true;
        mesg = "training success";
      } else {
        mesg = "training failed " + bolt->error();
      }
    }

    response.set_content(string_format(RESPONSE_STRUCTURE,
                                       status ? "true" : "false", mesg.c_str()),
                         "application/json");
  });
  std::cout << "listening " << fd << std::endl;
  server.listen(fd);
  return true;
}