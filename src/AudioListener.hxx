#ifndef BOLT_AUDIOLISTENER_HXX
#define BOLT_AUDIOLISTENER_HXX

#include "Listener.hxx"
#include <vosk_api.h>
#include <pipewire/pipewire.h>
#include <spa/param/audio/format-utils.h>
#include "json.hpp"
namespace bolt {

    class AudioListener : public Listener {
    private:
        VoskModel* model{nullptr};
        pw_main_loop* loop{nullptr};
        pw_stream* stream{nullptr};
        spa_audio_info format;

        VoskRecognizer *recognizer{nullptr};

    public:
        AudioListener(nlohmann::json const& config);
        ~AudioListener();

        std::string listen();
    };

} // bolt

#endif //BOLT_AUDIOLISTENER_HXX
