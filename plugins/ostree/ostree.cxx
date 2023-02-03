#include <array>
#include <cstdio>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

#include "../../src/Plugin.hxx"

#define CRITICAL_SYSTEM_COMMAND_REQUIREMENT                       \
    "critical system command need to be in specific pattern for " \
    "safety precautions, i.e. 'switch' ref:<ref>:<branch>"

std::tuple<int, std::string> exec(std::string cmd) {
    std::array<char, 128> buffer;
    std::string result;

    auto pipe = popen((cmd + " 2>&1").c_str(), "r");

    if (!pipe) throw std::runtime_error("popen() failed!");

    while (!feof(pipe)) {
        if (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
            result += buffer.data();
    }

    auto rc = pclose(pipe);
    if (result.size() && result.back() == '\n') result.pop_back();

    return {rc, result};
}

std::vector<std::string> getOSTreeRemotes() {
    auto [status, output] = exec("ostree remote refs");
    if (status != 0) {
        throw std::runtime_error("failed to retreive ostree ref");
    }
    std::vector<std::string> refs;
    std::stringstream ss(output);
    std::string ref;
    while (std::getline(ss, ref)) {
        refs.push_back(ref);
    }

    return refs;
}

PLUGIN_FUNCTION(main) { return false; }

PLUGIN_FUNCTION(update) {
    os << "checking and applying system updates." << std::endl;
    auto [status, mesg] = exec("pkexec ostree admin upgrade");
    os << mesg;
    return true;
}

PLUGIN_FUNCTION(switch) {
    if (ctxt->tokens.size() != 2) {
        return false;
    }
    std::string refstr = ctxt->tokens[1];

    auto idx = refstr.find(':', 4);
    if ((refstr.find("ref:", 0) != 0) || (idx == std::string::npos)) {
        return false;
    }
    refstr = refstr.substr(4);

    idx = refstr.find(':');

    std::string ref = refstr.substr(0, idx);
    std::string branch = refstr.substr(idx + 1);

    auto [status, arch] = exec("uname -m");

    std::string ostreeRef = ref + "/stable/" + arch + "-" + branch;
    if (branch.find('/') != std::string::npos) {
        ostreeRef = branch;
    }

    {
        os << "pulling ref " << ostreeRef << " from " << ref << std::endl;
        auto [status, output] = exec("ostree pull " + ref + " " + ostreeRef);
        if (status != 0) {
            throw std::runtime_error(output);
        }
    }

    {
        os << "deploying " << ostreeRef << std::endl;
        auto [status, output] =
            exec("ostree admin deploy " + ref + ":" + ostreeRef);
        if (status != 0) {
            throw std::runtime_error(output);
        }
    }
    os << "successfully switch to " << ostreeRef
       << ", reboot you system to boot into the new ostree";
    return true;
}