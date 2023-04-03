#include "StreamResponder.hxx"
#include <iostream>
#include <fstream>

namespace bolt {
    StreamResponder::StreamResponder(nlohmann::json const& config) {
        auto stream = config["stream"].get<std::string>();
        if (stream == "cout") {
            ostream = &std::cout;
        } else if (stream.starts_with("file:")) {
            ostream = new std::ofstream(stream.substr(6));
        } else {
            throw std::runtime_error("invalid output stream specified");
        }
    }

    StreamResponder::~StreamResponder() {
        if (ostream != &std::cout) {
            delete ostream;
        }
    }

    void StreamResponder::respond(const std::string &query) {
        ostream->write(query.c_str(), query.size());
        ostream->write("\n", 1);
    }
} // bolt