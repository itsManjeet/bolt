#ifndef BOLT_AUDIORESPONDER_HXX
#define BOLT_AUDIORESPONDER_HXX

#include "Responder.hxx"
namespace bolt {

    class AudioResponder : public Responder {
    private:
        std::string language{"English"};
    public:
        explicit AudioResponder(nlohmann::json const& config);
        void respond(std::string const& response);
    };

} // bolt

#endif //BOLT_AUDIORESPONDER_HXX
