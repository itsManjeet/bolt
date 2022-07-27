#include "shell.hh"
using namespace bolt::plugins;

#include "../../Debug.hh"

std::string Shell::respond(std::string intension, std::string data) {
  LOG("SHELL", "EXECUTING COMMAND = '" << data << "'");

  char buf[128];
  std::string result = "";
  FILE* pipe = popen(data.c_str(), "r");
  if (!pipe) {
    return "popen() failed, check logs";
  }

  try {
    while (fgets(buf, sizeof(buf), pipe) != nullptr) {
      result += buf;
    }
  } catch (...) {
    pclose(pipe);
    return "fgets() failed, check logs";
  }
  pclose(pipe);
  result = result.substr(0, result.size() - 1);
  LOG("SHELL", "OUTPUT = '" << result << "'");
  return result;
}