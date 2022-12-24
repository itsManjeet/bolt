#include "Bolt.hxx"
using namespace bolt;

#include "classifier/naive_bayes/NaiveBayes.hxx"
using namespace bolt::classifier;

#include <dlfcn.h>

#include <config.hxx>
#include <filesystem>

#include "nlp/Tokenizer.hxx"

Bolt::Bolt(std::string modelPath) : modelPath(modelPath) {
    classifier_ = std::make_unique<NaiveBayes>();
    classifier_->load(modelPath);
}

void Bolt::train(std::string trainFile) {
    classifier_->train(trainFile);
    classifier_->save(modelPath);
}

std::string Bolt::intension(std::string sentence) {
    auto [intension, score] = classifier_->classify(sentence);
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

    std::string pluginPath = std::filesystem::path(BOLT_PLUGIN_DIR) /
                             ("libBolt_" + pluginId + ".so");

    if (!std::filesystem::exists(pluginPath)) {
        throw std::runtime_error("missing required plugin " + pluginPath);
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