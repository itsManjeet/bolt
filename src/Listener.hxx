#ifndef BOLT_LISTENER_HXX
#define BOLT_LISTENER_HXX

#include <string>
#include "json.hpp"
namespace bolt {

    class Listener {
    public:
        virtual std::string listen() = 0;

        static Listener* create(nlohmann::json const& config);
    };

} // bolt

#endif //BOLT_LISTENER_HXX
