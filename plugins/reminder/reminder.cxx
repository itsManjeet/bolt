#include <Bolt.hxx>

using namespace bolt;

#include <nlp.hxx>

PLUGIN(reminder) {
    auto tokens = nlp::tokenize(sentence);
    nlp::clean(tokens, {"remember", "let", "suppose"});
    std::vector<std::string>::const_iterator idx;
    for (auto const &i: {"is", "as"}) {
        idx = std::find(tokens.begin(), tokens.end(), "is");
        if (idx != tokens.end()) {
            break;
        }
    }
    std::string var = nlp::join(tokens.begin(), idx - 1);
    std::string value = "true";
    if (idx != tokens.end()) {
        value = nlp::join(idx + 1, tokens.end());
    }
    bolt->remember(var, value);
    bolt->say("learnt that " + var + " is " + value);
    return true;
}