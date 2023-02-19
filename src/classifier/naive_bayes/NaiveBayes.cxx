#include "NaiveBayes.hxx"
using namespace bolt::classifier;

#include <algorithm>
#include <fstream>
#include <iostream>
#include <json.hpp>
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
                classWords[intension][token] = 0.0;
            }
            classWords[intension][token] += 1.0;
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
        if (iter->second.find(token) != iter->second.end()) {
            score +=
                std::log(classWords[intension][token] / corpusWords[token]);
        }
    }
    return score;
}

std::vector<std::tuple<std::string, float>> NaiveBayes::classify(
    std::string const& query) {
    std::vector<std::tuple<std::string, float>> solutions;
    float max_score = 0.0;
    for (auto const& intension : classWords) {
        auto score = calculate(query, intension.first);
        if (score > max_score) {
            max_score = score;
        }
        auto pos =
            std::find_if(solutions.begin(), solutions.end(),
                         [score](std::tuple<std::string, float> const& p) {
                             return std::get<1>(p) < score;
                         });
        solutions.insert(pos, {intension.first, score});
    }
    return solutions;
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
        classWords[i.key()] = std::map<std::string, float>();
        for (auto const& j : i.value().items()) {
            classWords[i.key()][j.key()] = j.value().get<float>();
        }
    }

    for (auto const& i : value["corpusWords"].items()) {
        corpusWords[i.key()] = i.value().get<float>();
    }
}