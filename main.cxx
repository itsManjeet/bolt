#include "config.hxx"
#include "src/Bolt.hxx"

#include <iostream>
#include "src/nlp.hxx"

int main(int argc, char **argv) {
    std::string config_file = "/etc/bolt.json";
    std::optional<std::string> training_file{};
    std::string sentence;
    for(int i = 1; i < argc; i++ ){
        if (argv[i][0] == '-') {
            if (!strcmp(argv[i], "-config")) {
                config_file = argv[++i];
            } else if (!strcmp(argv[i], "-train")) {
                training_file = argv[++i];
            } else {
                std::cerr << "ERROR: invalid argument " << argv[i] << std::endl;
                return 1;
            }
        } else {
            sentence += " ";
            sentence += argv[i];
        }
    }
    sentence = bolt::nlp::trim(sentence);
    if (sentence.empty()) sentence = "hello";

    auto bot = bolt::Bolt(config_file);
    if (training_file.has_value()) {
        bot.train(*training_file);
        std::cout << "SUCCESS" << std::endl;
        return 0;
    }

    if (!bot.init()) {
        return 1;
    }
    bot.respond(sentence);
    return 0;
}