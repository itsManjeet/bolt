#include "Responder.hxx"

#include "AudioResponder.hxx"
#include "StreamResponder.hxx"

namespace bolt {
    Responder *Responder::create(const nlohmann::json &config) {
        auto type = config["type"].get<std::string>();
        if (type == "stream") {
            return new StreamResponder(config);
        } else if (type == "audio") {
            return new AudioResponder(config);
        }
        return nullptr;
    }
} // bolt