#include "AudioListener.hxx"
#include <memory>
#include <iostream>


namespace bolt {

    AudioListener::AudioListener(nlohmann::json const& config) {
        model = vosk_model_new(config["model"].get<std::string>().c_str());
        recognizer = vosk_recognizer_new(model, config["sample-rate"].get<double>());
        vosk_recognizer_set_max_alternatives(recognizer, 3);
        vosk_recognizer_set_words(recognizer, true);
    }

    AudioListener::~AudioListener() {
        vosk_recognizer_free(recognizer);
        vosk_model_free(model);
    }

    static void do_quit(void *loop, int sig) {
        pw_main_loop_quit(reinterpret_cast<pw_main_loop *>(loop));
    }

    std::string AudioListener::listen() {
        return "not yet supported";
    }
} // bolt