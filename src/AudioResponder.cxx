#include "AudioResponder.hxx"
#include <espeak-ng/espeak_ng.h>

namespace bolt {
    void AudioResponder::respond(const std::string &response) {
        char *path = nullptr;
        int buflen = 500, options = 0;
        unsigned int *identifier;
        void *userData;
        unsigned int position = 0, endPosition = 0, flags = espeakCHARS_AUTO;
        espeak_POSITION_TYPE positionType = espeak_POSITION_TYPE(0);
        espeak_Initialize(AUDIO_OUTPUT_SYNCH_PLAYBACK, buflen, path, options);
        espeak_SetVoiceByName(language.c_str());
        espeak_Synth((const void *) response.c_str(), response.size(), position, positionType, endPosition, flags,
                     identifier, userData);
    }

    AudioResponder::AudioResponder(nlohmann::json const &config) {
        language = config["language"].get<std::string>();
    }
} // bolt