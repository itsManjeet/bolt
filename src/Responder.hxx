#ifndef BOLT_RESPONDER_HXX
#define BOLT_RESPONDER_HXX

#include <string>
#include "json.hpp"

namespace bolt {

    class Responder {
    public:
        virtual void respond(std::string const &response) = 0;

        static Responder* create(nlohmann::json const& config);
    };

} // bolt

#endif //BOLT_RESPONDER_HXX
