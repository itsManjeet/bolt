#include "Bolt.hxx"
using namespace bolt;

#include "classifier/naive_bayes/NaiveBayes.hxx"
using namespace bolt::classifier;

#include <dlfcn.h>

#include <config.hxx>
#include <filesystem>
#include <iostream>
#include <optional>

#include "nlp/Tokenizer.hxx"

#define DEBUG(...) \
    if (config->debug) std::cerr << "DEBUG: " << __VA_ARGS__ << std::endl;

Bolt::Bolt(Config const* config) : config{config} {
    classifier_ = std::make_unique<NaiveBayes>();
    DEBUG("loading model '" << config->model << "'");
    classifier_->load(config->model);
}

void Bolt::train(std::string trainFile) {
    classifier_->train(trainFile);
    DEBUG("saving model '" << config->model << "'");
    classifier_->save(config->model);
}

std::vector<std::tuple<std::string, double>> Bolt::intentions(
    std::string sentence) {
    std::vector<std::string> words = nlp::tokenize(sentence);

    // for using specific intention
    if (words.size() && words[0][0] == '@') {
        return {{words[0].substr(1), 100.00}};
    }
    return classifier_->classify(sentence);
}

PluginFun Bolt::getPluginFunction(const std::string& intention, Context* ctxt) {
    std::string funPrefix = "main";

    std::string pluginId = intention;
    auto idx = pluginId.find_first_of(':');

    if (idx != std::string::npos) {
        funPrefix = intention.substr(idx + 1);
        pluginId = pluginId.substr(0, idx);
    }

    std::optional<Plugin> plugin;

    for (auto path : config->plugin_path) {
        auto pluginPath =
            std::filesystem::path(path) / ("libBolt_" + pluginId + ".so");

        if (std::filesystem::exists(pluginPath) &&
            std::filesystem::is_regular_file(pluginPath)) {
            DEBUG("found plugin '" << pluginPath
                                   << "' of type 'DynamicModule'");
            plugin = Plugin{Plugin::Type::DynamicModule, pluginPath};
            break;
        }

        for (auto const& ext : std::map<std::string, Plugin::Type>{
                 {".py", Plugin::Type::PythonScript},
                 {".sh", Plugin::Type::ShellScript},
                 {"", Plugin::Type::Executable},
             }) {
            pluginPath = std::filesystem::path(path) / (pluginId + ext.first);
            if (std::filesystem::exists(pluginPath) &&
                std::filesystem::is_regular_file(pluginPath)) {
                plugin = Plugin{ext.second, pluginPath};
                break;
            }
        }
    }
    if (!plugin.has_value()) {
        throw std::runtime_error("missing required plugin '" + pluginId + "'");
    }
    PluginFun fun;
    ctxt->plugin = plugin->path;
    switch (plugin->type) {
        case Plugin::Type::Executable:
        case Plugin::Type::ShellScript:
        case Plugin::Type::PythonScript:
            fun = [](Context* ctxt, std::ostream& os) -> bool {
                setenv("BOLT_IS_QUESTION", ctxt->isQuestion ? "1" : "0", 1);
                setenv("BOLT_PLUGIN_PATH", ctxt->plugin.c_str(), 1);
                setenv("BOLT_RAW_SENTENCE", ctxt->rawSentence.c_str(), 1);
                std::string command = ctxt->plugin;
                for (auto const& c : ctxt->tokens) {
                    command += " " + c;
                }
                system(command.c_str());
                return true;
            };
            break;
        case Plugin::Type::DynamicModule: {
            if (handlers.find(plugin->path) == handlers.end()) {
                auto handler = dlopen(plugin->path.c_str(), RTLD_LAZY);
                if (handler == nullptr) {
                    throw std::runtime_error(dlerror());
                }
                handlers[plugin->path] = handler;
            }

            fun = PluginFun((bool (*)(Context*, std::ostream&))(
                dlsym(handlers[plugin->path], ("Bolt_" + funPrefix).c_str())));
            if (fun == nullptr) {
                throw std::runtime_error(dlerror());
            }
        } break;
    }

    return fun;
}

void Bolt::respond(std::string sentence, std::ostream& os) {
    auto _intentions = intentions(sentence);
    if (config->debug) {
        DEBUG("THRESHOLD: " << config->threshold);
        for (auto c : _intentions) {
            DEBUG("INTENSION: " << std::get<0>(c)
                                << " SCORE: " << (float)std::get<1>(c));
        }
    }
    PluginFun fun;

    context.isQuestion = false;
    context.rawSentence = sentence;
    context.tokens = nlp::tokenize(sentence);
    context.config = config;
    if (context.previous.size() >= 10) {
        context.previous.pop_front();
    }

    context.previous.push_back({sentence, ""});

    for (auto const& i : _intentions) {
        std::string plugin_id = std::get<0>(i);
        if (std::get<1>(i) < config->threshold) {
            plugin_id = "unknown";
        }
        try {
            DEBUG("EXECUTING PLUGIN " << plugin_id);
            fun = getPluginFunction(plugin_id, &context);
        } catch (std::runtime_error const& error) {
            os << "ERROR: " << error.what();
            return;
        }

        try {
            if (fun(&context, os)) {
                return;
            }
        } catch (std::exception const& exc) {
            os << "ERROR: " << exc.what();
        }

        if (plugin_id == "unknown") {
            break;
        }
    }
}