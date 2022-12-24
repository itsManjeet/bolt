#include "NaiveBayes.hxx"
using namespace bolt::classifier;

#include <algorithm>
#include <fstream>
#include <nlohmann/json.hpp>
using namespace nlohmann;
#include <sstream>

#include "../../nlp/Tokenizer.hxx"

void NaiveBayes::train(std::vector<std::string>::const_iterator iter,
                       std::vector<std::string>::const_iterator end,
                       std::string intension) {
    while (iter != end) {
        for (auto const& token : nlp::tokenize(*iter)) {
            if (corpusWords.find(token) == corpusWords.end()) {
                corpusWords[token] = 0.0;
            } else {
                corpusWords[token] += 1.0;
            }

            if (classWords.find(intension) == classWords.end()) {
                classWords[intension] = std::vector<std::string>();
            }
            if (std::find(classWords[intension].begin(),
                          classWords[intension].end(),
                          token) == classWords[intension].end()) {
                classWords[intension].push_back(token);
            }
        }
        iter++;
    }
}

float NaiveBayes::calculate(std::string const& sentence,
                            std::string const& intension) {
    float score = 0.0;
    auto iter = classWords.find(intension);
    if (iter == classWords.end()) {
        return score;
    }

    for (auto const& token : nlp::tokenize(sentence)) {
        if (std::find(iter->second.begin(), iter->second.end(), token) !=
            iter->second.end()) {
            score += (1.0 / corpusWords[token]);
        }
    }
    return score;
}

std::tuple<std::string, float> NaiveBayes::classify(std::string const& query) {
    float max_score = 0.0;
    std::string targetIntension = "unknown";

    for (auto const& intension : classWords) {
        auto score = calculate(query, intension.first);
        if (score > max_score) {
            max_score = score;
            targetIntension = intension.first;
        }
    }
    return {targetIntension, max_score};
}

void NaiveBayes::save(std::string outputfile) {
    nlohmann::json json = {
        {"classWords", classWords},
        {"corpusWords", corpusWords},
    };

    std::ofstream writer(outputfile, std::ios_base::binary);
    writer << json.dump() << std::flush;
}

void NaiveBayes::load(std::string modelfile) {
    std::ifstream reader(modelfile);
    if (!reader.good()) {
        throw std::runtime_error("failed to read model file '" + modelfile +
                                 "'");
    }
    auto value = json::parse(reader);

    classWords.clear();
    corpusWords.clear();

    for (auto const& i : value["classWords"].items()) {
        classWords[i.key()] = std::vector<std::string>();
        for (auto const& j : i.value()) {
            classWords[i.key()].push_back(j.get<std::string>());
        }
    }

    for (auto const& i : value["corpusWords"].items()) {
        corpusWords[i.key()] = i.value().get<float>();
    }
}