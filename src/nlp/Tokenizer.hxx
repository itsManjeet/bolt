#ifndef __BOLT_NLP_TOKENIZER_HXX__
#define __BOLT_NLP_TOKENIZER_HXX__

#include <sstream>
#include <string>
#include <vector>

namespace bolt::nlp {

static std::vector<std::string> tokenize(std::string const& query) {
    std::stringstream ss(query);
    std::string token;
    std::vector<std::string> tokens;

    static std::vector<std::string> skippableWords = {
        "to", "is", "am", "are", ".",    ",",
        ":",  "on", "in", "the", "this", "that",
    };
    while (std::getline(ss, token, ' ')) {
        if (token.size() == 0) continue;
        if (std::find(skippableWords.begin(), skippableWords.end(), token) !=
            skippableWords.end()) {
            continue;
        }
        std::transform(token.begin(), token.end(), token.begin(), tolower);
        tokens.push_back(token);
    }
    return tokens;
}
}  // namespace bolt::nlp

#endif