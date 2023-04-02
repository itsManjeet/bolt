#ifndef BOLT_STREAMRESPONDER_HXX
#define BOLT_STREAMRESPONDER_HXX

#include "Responder.hxx"
#include <ostream>

namespace bolt {

    class StreamResponder : public Responder {
    private:
        std::ostream *ostream;

    public:
        explicit StreamResponder(nlohmann::json const& config);
        virtual ~StreamResponder();

        void respond(std::string const& query);
    };

} // bolt

#endif //BOLT_STREAMRESPONDER_HXX
