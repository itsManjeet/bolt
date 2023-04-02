#ifndef __BOLT_NLP_TOKENIZER_HXX__
#define __BOLT_NLP_TOKENIZER_HXX__

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

namespace bolt::nlp {

    static std::vector<std::string> tokenize(std::string const &query) {
        std::stringstream ss(query);
        std::string token;
        std::vector<std::string> tokens;

        static std::vector<std::string> skippableWords = {
                "to", "is", "am", "are", ".", ",", ":",
                "on", "in", "the", "this", "that", "my",
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

    static std::string trim(const std::string &str) {
        auto start = str.begin();
        while (start != str.end() && std::isspace(*start)) {
            ++start;
        }

        auto end = str.end();
        do {
            --end;
        } while (std::distance(start, end) > 0 && std::isspace(*end));

        return std::string(start, end + 1);
    }
}  // namespace bolt::nlp

#endif