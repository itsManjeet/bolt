#include <Bolt.hxx>

using namespace bolt;

#include <nlp.hxx>

PLUGIN(reminder) {
    auto tokens = nlp::tokenize(sentence, false);
    if (nlp::contains_any(sentence, {"remember", "let", "suppose"})) {
        nlp::clean(tokens, {"remember", "let", "suppose"});
        auto idx = tokens.begin() + 1;
        for (auto const &i: {"is", "as"}) {
            idx = std::find(tokens.begin(), tokens.end(), "is");
            if (idx != tokens.end()) {
                break;
            }
        }
        std::string var = nlp::join(tokens.begin(), idx);
        std::string value = "true";
        if (idx != tokens.end()) {
            value = nlp::join(idx + 1, tokens.end());
        }
        bolt->remember(var, value);
        bolt->say("learnt that " + var + " is " + value);
    } else if (nlp::contains_any(sentence, {"what", "who"})) {
        nlp::clean(tokens, {"what", "who", "is"});
        std::string quest = nlp::trim(nlp::join(tokens.begin(), tokens.end()));
        bolt->say(bolt->recall<std::string>(quest, "Sorry i have no idea about that"));
    } else {
        return false;
    }

    return true;
}