#ifndef __BOLT_PLUGIN_HXX__
#define __BOLT_PLUGIN_HXX__

#include <list>
#include <ostream>
#include <string>
#include <vector>

struct Context {
    std::string rawSentence;
    std::vector<std::string> tokens;
    std::list<std::pair<std::string, std::string>> previous;
    bool isQuestion;
};

using PluginFun = void (*)(Context*, std::ostream& os);

#define PLUGIN_FUNCTION(funId) \
    extern "C" void Bolt_##funId(Context* ctxt, std::ostream& os)

#endif