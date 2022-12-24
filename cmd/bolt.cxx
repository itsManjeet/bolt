#include "../src/Bolt.hxx"

#include <config.hxx>
#include <iostream>

#include "../src/Plugin.hxx"

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cerr << "Usage: " << argv[0] << " <message>" << std::endl;
        return 1;
    }
    std::string sentence;
    std::string sep;
    for (int i = 1; i < argc; i++) {
        sentence += sep + argv[i];
        sep = " ";
    }
    bolt::Bolt bot(BOLT_DATA_DIR "/model.json");
    bot.respond(sentence, std::cout);

    return 0;
}