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

std::string Bolt::intension(std::string sentence) {
    std::vector<std::string> words = nlp::tokenize(sentence);
    auto [intension, score] = classifier_->classify(sentence);

    DEBUG("INTENSION: '" << intension << "'");
    DEBUG("SCORE: '" << score << "'");

    DEBUG("THRESHOLD: '" << config->threshold << "'");

    if (score < config->threshold) {
        DEBUG("SCORE BELOW THRESHOLD");
        intension = "unknown";
    }
    return intension;
}

PluginFun Bolt::getPluginFunction(std::string intension) {
    std::string funPrefix = "main";

    std::string pluginId = intension;
    auto idx = pluginId.find_first_of(':');

    if (idx != std::string::npos) {
        funPrefix = intension.substr(idx + 2);
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
    auto pluginId = intension(sentence);
    PluginFun fun;

    try {
        fun = getPluginFunction(pluginId);
    } catch (std::runtime_error const& error) {
        os << "ERROR: " << error.what();
        return;
    }

    context.isQuestion = false;
    context.rawSentence = sentence;
    context.tokens = nlp::tokenize(sentence);
    context.config = config;
    if (context.previous.size() >= 10) {
        context.previous.pop_front();
    }
    context.previous.push_back({sentence, ""});
    try {
        fun(&context, os);
    } catch (std::exception const& exc) {
        os << "ERROR: " << exc.what();
    }
}