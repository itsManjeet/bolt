#include "StreamListener.hxx"
#include <iostream>
#include <fstream>

namespace bolt {
    StreamListener::StreamListener(nlohmann::json const &config) {
        auto stream = config["stream"].get<std::string>();
        if (stream == "cin") {
            istream = &std::cin;
        } else if (stream.starts_with("file:")) {
            istream = new std::fstream(stream.substr(6));
        } else {
            throw std::runtime_error("invalid stream specified");
        }
    }

    std::string StreamListener::listen() {
        std::string line;
        std::getline(*istream, line, '\n');
        return line;
    }

    StreamListener::~StreamListener() {
        if (istream != &std::cin) {
            delete istream;
        }
    }
} // bolt