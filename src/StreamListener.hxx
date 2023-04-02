#ifndef BOLT_STREAMLISTENER_HXX
#define BOLT_STREAMLISTENER_HXX

#include "Listener.hxx"
#include <istream>

namespace bolt {

    class StreamListener : public Listener {
        std::istream* istream;
    public:
        StreamListener(nlohmann::json const& config);
        virtual ~StreamListener();

        std::string listen();
    };

} // bolt

#endif //BOLT_STREAMLISTENER_HXX
