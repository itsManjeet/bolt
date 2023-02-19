#ifndef __BOLT_PLUGIN_HXX__
#define __BOLT_PLUGIN_HXX__

#include <functional>
#include <list>
#include <ostream>
#include <string>
#include <vector>

#include "config/config.hxx"
struct Context {
    std::string plugin;
    std::string rawSentence;
    std::vector<std::string> tokens;
    std::list<std::pair<std::string, std::string>> previous;
    bolt::Config const* config;
    bool isQuestion;
};

struct Plugin {
    enum class Type {
        DynamicModule,
        ShellScript,
        PythonScript,
        Executable
    } type;
    std::string path;
};

using PluginFun = bool (*)(Context*, std::ostream& os);

#define PLUGIN_FUNCTION(funId) \
    extern "C" bool Bolt_##funId(Context* ctxt, std::ostream& os)

#endif