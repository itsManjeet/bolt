#include <algorithm>
#include <numeric>

#include "../../src/Plugin.hxx"

std::vector<std::string> HOT_WORDS = {"search",  "where", "find",     "file",
                                      "library", "doc",   "document", "bin",
                                      "binary",  "app"};

PLUGIN_FUNCTION(main) {
    bool hint_is_file = false, hint_is_binary;
    std::string search_pattern, sep;
    for (auto const& i : HOT_WORDS) {
        if (i == "file" || i == "doc" || i == "document") {
            hint_is_file = true;
        } else if (i == "bin" || i == "binary" || i == "app") {
            hint_is_binary = true;
        }
        auto iter = std::find(ctxt->tokens.begin(), ctxt->tokens.end(), i);
        if (iter != ctxt->tokens.end()) {
            ctxt->tokens.erase(iter);
        }
    }

    if (ctxt->tokens.size() == 0) {
        os << "please provide the ";
        if (hint_is_file) {
            os << "file";
        } else if (hint_is_binary) {
            os << "app or binary ";
        }
        os << "name";
        return true;
    }

    search_pattern = std::accumulate(
        ctxt->tokens.begin(), ctxt->tokens.end(), std::string{},
        [&sep](std::string& s, const std::string& piece) -> decltype(auto) {
            s += sep + piece;
            sep = " ";
            return s;
        });

    // TODO: handle search in better way
    os << "searching " << search_pattern << std::endl;
    system(("locate " + search_pattern).c_str());
    return true;
}