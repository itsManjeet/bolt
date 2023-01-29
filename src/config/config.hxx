#ifndef __BOLT_CONFIGURATION_HXX__
#define __BOLT_CONFIGURATION_HXX__

#include <string>
#include <vector>
namespace bolt {
struct Config {
    std::string model;
    std::vector<std::string> responses;
    std::vector<std::string> plugin_path;
    float threshold;
    bool debug;
};
}  // namespace bolt

#endif