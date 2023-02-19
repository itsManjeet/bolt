#ifndef __BOLT_HXX__
#define __BOLT_HXX__

#include <functional>
#include <map>
#include <memory>
#include <string>

#include "Plugin.hxx"
#include "classifier/Classifier.hxx"
#include "config/config.hxx"
namespace bolt {

class Bolt {
   private:
    std::map<std::string, void*> handlers;
    std::unique_ptr<classifier::Classifier> classifier_;
    Config const* config;
    Context context;

    PluginFun getPluginFunction(std::string intension, Context* ctxt);

   public:
    Bolt(Config const* config);

    void train(std::string trainFile);

    std::vector<std::tuple<std::string, float>> intensions(
        std::string sentence);

    void respond(std::string sentence, std::ostream& os);
};

}  // namespace bolt

#endif