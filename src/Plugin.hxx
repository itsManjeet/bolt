#ifndef __BOLT_PLUGIN_HXX__
#define __BOLT_PLUGIN_HXX__

#include <list>
#include <ostream>
#include <string>
#include <vector>

#include "config/config.hxx"
struct Context {
    std::string rawSentence;
    std::vector<std::string> tokens;
    std::list<std::pair<std::string, std::string>> previous;
    bolt::Config const* config;
    bool isQuestion;
};

using PluginFun = bool (*)(Context*, std::ostream& os);

#define PLUGIN_FUNCTION(funId) \
    extern "C" bool Bolt_##funId(Context* ctxt, std::ostream& os)

#endif