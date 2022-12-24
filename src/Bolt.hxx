#ifndef __BOLT_HXX__
#define __BOLT_HXX__

#include <functional>
#include <map>
#include <memory>
#include <string>

#include "Plugin.hxx"
#include "classifier/Classifier.hxx"

namespace bolt {

class Bolt {
   private:
    std::map<std::string, void*> handlers;
    std::unique_ptr<classifier::Classifier> classifier_;
    std::string modelPath;
    Context context;

    PluginFun getPluginFunction(std::string intension);

   public:
    Bolt(std::string modelPath);

    void train(std::string trainFile);

    std::string intension(std::string sentence);

    void respond(std::string sentence, std::ostream& os);
};

}  // namespace bolt

#endif