#ifndef __BOLT_NLP_TOKENIZER_HXX__
#define __BOLT_NLP_TOKENIZER_HXX__

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>

namespace bolt::nlp {

    // TODO: added tagging?
    static std::vector<std::string> tokenize(std::string const &query, bool skip=true) {
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
                skippableWords.end() && skip) {
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

        auto dist = std::distance(start, end + 1);
        if (dist == 0) return "";
        auto r = std::string(start, end + 1);
        std::transform(r.begin(), r.end(), r.begin(), [](char c) -> char {
            return tolower(c);
        });
        r.erase(std::remove_if(r.begin(), r.end(), [](char c) {
            return !isascii(c);
        }), r.end());
        return r;
    }

    static bool contains_any(std::string const &sentence, std::vector<std::string> const &tokens) {
        for (auto const &i: tokens) {
            if (sentence.find(i) != std::string::npos) {
                return true;
            }
        }
        return false;
    }

    static bool contains(std::string const &sentence, std::vector<std::string> const &tokens) {
        for (auto const &i: tokens) {
            if (sentence.find(i) == std::string::npos) {
                return false;
            }
        }
        return true;
    }

    static bool clean(std::vector<std::string> &tokens, std::vector<std::string> const &words) {
        for (auto const &word: words) {
            tokens.erase(std::remove_if(tokens.begin(), tokens.end(), [word](auto const &w) -> bool {
                return w == word;
            }), tokens.end());
        }
        return true;
    }

    static std::time_t getTime(std::string const &s) {
        std::tm tm;
        if (std::count(s.begin(), s.end(), ':') == 3) {
            strptime(s.c_str(), "%H:%M:%S", &tm);
            return mktime(&tm);
        } else if (std::count(s.begin(), s.end(), ':') == 2) {

        }
    }

    static std::string
    join(std::vector<std::string>::const_iterator begin, std::vector<std::string>::const_iterator end) {
        std::stringstream ss;
        std::string sep;
        for (auto i = begin; i != end; i++) {
            ss << sep << *i;
            sep = " ";
        }
        return ss.str();
    }
}  // namespace bolt::nlp

#endif