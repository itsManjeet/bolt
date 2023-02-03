#include "../src/Bolt.hxx"

#include <string.h>

#include <config.hxx>
#include <filesystem>
#include <iostream>

#include "../src/Plugin.hxx"

int main(int argc, char** argv) {
    bolt::Config config{
        .model = BOLT_DATA_DIR "/model.json",
        .responses = {BOLT_DATA_DIR "/responses.txt"},
        .plugin_path = {BOLT_PLUGIN_DIR},
        .threshold = 0.02,
    };
    std::string training_txt;
    std::string sentence, sep;
    // parse input arguments
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            argv[i]++;
            if (!strcmp(argv[i], "model")) {
                if (argc <= i + 1) {
                    std::cerr << "ERROR: no model path specified" << std::endl;
                    return 1;
                }
                config.model = argv[++i];
            } else if (!strcmp(argv[i], "responses")) {
                if (argc <= i + 1) {
                    std::cerr << "ERROR: no responses path specified"
                              << std::endl;
                    return 1;
                }
                config.responses.push_back(argv[++i]);
            } else if (!strcmp(argv[i], "plugin-path")) {
                if (argc <= i + 1) {
                    std::cerr << "ERROR: no plugin path specified" << std::endl;
                    return 1;
                }
                config.plugin_path.push_back(argv[++i]);
            } else if (!strcmp(argv[i], "threshold")) {
                if (argc <= i + 1) {
                    std::cerr << "ERROR: no thresold value specified"
                              << std::endl;
                    return 1;
                }
                config.threshold = std::stoi(argv[++i]);
            } else if (!strcmp(argv[i], "train")) {
                if (argc <= i + 1) {
                    std::cerr << "ERROR: missing required arguments "
                                 "<model.json>"
                              << std::endl;
                    return 1;
                }

                training_txt = argv[++i];
            } else if (!strcmp(argv[i], "debug")) {
                config.debug = true;
            } else {
                std::cerr << "ERROR: invalid flag provided '-" << argv[i] << "'"
                          << std::endl;
                return 1;
            }
        } else {
            sentence += sep + argv[i];
            sep = " ";
        }
    }

    if (training_txt.size() && !std::filesystem::exists(training_txt)) {
        std::cerr << "ERROR: missing training data at '" << training_txt << "'"
                  << std::endl;
        return 1;
    }

    if (sentence.length() == 0) {
        sentence = "hey";
    }

    bolt::Bolt bot(&config);

    if (training_txt.size()) {
        bot.train(training_txt);
    }

    if (!std::filesystem::exists(config.model)) {
        std::cerr << "ERROR: missing bolt model at '" << config.model << "'"
                  << std::endl;
        return 1;
    }

    bot.respond(sentence, std::cout);
    std::cout << std::endl;

    return 0;
}