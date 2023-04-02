#include "Listener.hxx"

#include "AudioListener.hxx"
#include "StreamListener.hxx"

namespace bolt {
    Listener *Listener::create(const nlohmann::json &config) {
        auto type = config["type"].get<std::string>();
        if (type == "audio") {
            return new AudioListener(config);
        } else if (type == "stream") {
            return new StreamListener(config);
        }
        return nullptr;
    }
} // bolt