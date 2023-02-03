#include "Bolt.hxx"
using namespace bolt;

#include "classifier/naive_bayes/NaiveBayes.hxx"
using namespace bolt::classifier;

#include <dlfcn.h>

#include <config.hxx>
#include <filesystem>
#include <iostream>

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

std::vector<std::tuple<std::string, float>> Bolt::intensions(
    std::string sentence) {
    std::vector<std::string> words = nlp::tokenize(sentence);
    return classifier_->classify(sentence);
}

PluginFun Bolt::getPluginFunction(std::string intension) {
    std::string funPrefix = "main";

    std::string pluginId = intension;
    auto idx = pluginId.find_first_of(':');

    if (idx != std::string::npos) {
        funPrefix = intension.substr(idx + 1);
        pluginId = pluginId.substr(0, idx);
    }

    std::string pluginPath;
    bool found = false;

    for (auto path : config->plugin_path) {
        pluginPath =
            std::filesystem::path(path) / ("libBolt_" + pluginId + ".so");

        if (std::filesystem::exists(pluginPath)) {
            found = true;
            break;
        }
    }

    if (!found) {
        throw std::runtime_error("missing required plugin '" + pluginId + "'");
    }

    if (handlers.find(pluginPath) == handlers.end()) {
        auto handler = dlopen(pluginPath.c_str(), RTLD_LAZY);
        if (handler == nullptr) {
            throw std::runtime_error(dlerror());
        }
        handlers[pluginPath] = handler;
    }

    PluginFun fun =
        (PluginFun)dlsym(handlers[pluginPath], ("Bolt_" + funPrefix).c_str());
    if (fun == nullptr) {
        throw std::runtime_error(dlerror());
    }

    return fun;
}

void Bolt::respond(std::string sentence, std::ostream& os) {
    auto _intensions = intensions(sentence);
    if (config->debug) {
        DEBUG("THRESHOLD: " << config->threshold);
        for (auto c : _intensions) {
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

    for (auto const& i : _intensions) {
        std::string plugin_id = std::get<0>(i);
        if (std::get<1>(i) < config->threshold) {
            plugin_id = "unknown";
        }
        try {
            fun = getPluginFunction(plugin_id);
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