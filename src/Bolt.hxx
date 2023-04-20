#ifndef BOLT_BOLT_HXX
#define BOLT_BOLT_HXX

#include "Model.hxx"
#include "Responder.hxx"
#include "Listener.hxx"
#include "json.hpp"
#include <variant>

using namespace nlohmann;

#include <memory>

namespace bolt {

    class Bolt {
    private:
        Model *model{nullptr};
        Listener *listener{nullptr};
        Responder *responder{nullptr};
        json config;
        std::string error_;
        json knowlegde;

        bool execute(std::string intension, std::string const &sentence);

    public:
        Bolt(std::string const &configFile);

        virtual ~Bolt();

        bool init();

        std::string listen() { return listener->listen(); }

        void say(std::string const &response) { responder->respond(response); }

        void say(std::vector<std::string> const &responses);

        void respond(std::string const &sentence);

        void train(std::string const &trainingFile, const std::string &outputModel);

        template<typename T>
        void remember(std::string const &id, T value) {
            knowlegde[id] = value;
        }


        template<typename T>
        T recall(std::string const &id, T fallback) {
            if (knowlegde.contains(id)) {
                return knowlegde[id].get<T>();
            }
            return fallback;
        }

    };

} // bolt

#define PLUGIN(id) constexpr auto PLUGIN_ID = #id; \
extern "C" bool BOLT_PLUGIN(std::string const& sentence, bolt::Bolt* bolt)

#endif //BOLT_BOLT_HXX
