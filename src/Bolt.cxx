#include "Bolt.hxx"

using namespace bolt;

#include <iostream>
#include <random>
#include <fstream>
#include <dlfcn.h>

Bolt::Bolt(std::string const &configfile) {
    std::ifstream reader(configfile);
    if (!reader.good()) {
        throw std::runtime_error("failed to read configuration file '" + configfile + "'");
    }
    reader >> config;
    reader.close();
}

Bolt::~Bolt() {
    std::ofstream knowledge_writer(std::filesystem::path(getenv("HOME")) / ".knowledge.json");
    knowledge_writer << knowlegde;

    if (model != nullptr)
        model->save(config["model"].get<std::string>());
}


void Bolt::respond(const std::string &sentence) {
    auto intensions = model->classify(sentence);
    auto [responses, responseScore] = model->fuzzySearch(sentence);
    responseScore *= 0.1; // decrese the priority of fuzzySearch
    if (!knowlegde.contains("history")) knowlegde["history"] = std::vector<std::string>();
    knowlegde["history"].push_back("-" + sentence);

    if (intensions.size() && responseScore < 0.8) {
        for (auto const &i: intensions) {
            if (execute(i.first, sentence)) {
                knowlegde["history"].push_back("$" + i.first);
                return;
            }
        }
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, responses.size() - 1);
    auto idx = dis(gen);
    say(responses[idx]);
}

bool Bolt::execute(std::string intension, const std::string &sentence) {
    for (auto const &i: config["plugin-path"].items()) {
        auto p = i.value().get<std::string>();
        auto plugin_path = std::filesystem::path(p) / intension;
        if (std::filesystem::exists(plugin_path)) {
            return WEXITSTATUS(system((plugin_path.string() + " " + sentence).c_str())) == 0;
        }

        plugin_path += ".so";
        if (std::filesystem::exists(plugin_path)) {
            auto handler = dlopen(plugin_path.c_str(), RTLD_LAZY | RTLD_LOCAL);
            if (handler == nullptr) {
                std::cerr << "ERROR: failed to load '" << plugin_path << std::endl;
                return false;
            }
            std::shared_ptr<void> _(nullptr, [&](...) {
                dlclose(handler);
            });

            auto fun = (bool (*)(std::string, Bolt *)) dlsym(handler, "BOLT_PLUGIN");
            if (fun == nullptr) {
                std::cerr << "ERROR: invalid plugin '" << plugin_path << std::endl;
                return false;
            }
            return fun(sentence, this);
        }
    }
    return false;
}

bool Bolt::init() {
    auto listener_config = config.find("listener");
    if (listener_config == config.end()) {
        error_ = "missing listener configuration";
        return false;
    }
    listener = Listener::create(listener_config.value());

    auto responder_config = config.find("responder");
    if (responder_config == config.end()) {
        error_ = "missing responder config";
        return false;
    }
    responder = Responder::create(responder_config.value());

    model = new Model();
    if (config.contains("train")) {
        model->train(config["train"].get<std::string>());
    } else {
        model->load(config["model"].get<std::string>());
    }

    auto knowledge_file = std::filesystem::path(getenv("HOME")) / ".knowledge.json";
    if (std::filesystem::exists(knowledge_file)) {
        auto reader = std::ifstream(knowledge_file);
        reader >> knowlegde;
    }

    return true;
}


void Bolt::say(const std::vector<std::string> &responses) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, responses.size() - 1);
    auto idx = dis(gen);
    auto resp = responses[idx];
    say(resp);
    knowlegde["history"].push_back(resp);
}

void Bolt::train(const std::string &trainingFile, const std::string &outputModel) {
    delete model;
    model = new Model();
    model->train(trainingFile);
    model->save(outputModel);
    delete model;
    model = nullptr;
}

