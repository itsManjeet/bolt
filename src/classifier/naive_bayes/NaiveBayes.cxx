#include "NaiveBayes.hxx"
using namespace bolt::classifier;

#include <algorithm>
#include <fstream>
#include <iostream>
#include <json.hpp>
using namespace nlohmann;
#include <sstream>

#include "../../nlp/Tokenizer.hxx"

void NaiveBayes::train(std::vector<std::string>::const_iterator text_begin,
                       std::vector<std::string>::const_iterator text_end,
                       std::vector<std::string>::const_iterator label_begin,
                       std::vector<std::string>::const_iterator label_end) {
    for(auto text = text_begin; text != text_end; text++) {
        auto label = *label_begin++;
        if (classCount.find(label) == classCount.end()) {
            classes.push_back(label);
            classCount[label] = 0;
        }
        classCount[label]++;

        for(auto const& word : nlp::tokenize(*text)) {
            if (vocabulary.find(word) == vocabulary.end()) {
                vocabulary[word] = 0;
            }
            if (wordCount[label].find(word) == wordCount[label].end()) {
                wordCount[label][word] = 0;
            }
            vocabulary[word]++;
            wordCount[label][word]++;
        }
    }
}

std::vector<std::tuple<std::string, double>> NaiveBayes::classify(
    std::string const& query) {
    std::vector<std::tuple<std::string, double>> solutions;
    double max_score = -INFINITY;
    for (auto const& intension : classes) {
        auto score = log(classCount[intension]);
        for (auto const& word : nlp::tokenize(query)) {
            if (wordCount[intension].find(word) != wordCount[intension].end()) {
                score += log(wordCount[intension][word]+1) - log(classCount[intension] + vocabulary.size());
            } else {
                score += log(1) - log(classCount[intension] + vocabulary.size());
            }
        }
        if (score > max_score) {
            max_score = score;
        }
        auto pos =
            std::find_if(solutions.begin(), solutions.end(),
                         [score](std::tuple<std::string, float> const& p) {
                             return std::get<1>(p) < score;
                         });
        solutions.insert(pos, {intension, score});
    }
    return solutions;
}

void NaiveBayes::save(std::string outputfile) {
    nlohmann::json json = {
        {"vocabulary", vocabulary},
        {"classCount", classCount},
        {"wordCount", wordCount},
        {"classes", classes},
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

    for (auto const& i : value["vocabulary"].items()) {
        vocabulary[i.key()] = i.value().get<int>();
    }

    for (auto const& i : value["classCount"].items()) {
        classCount[i.key()] = i.value().get<int>();
    }

    for(auto const& i : value["classes"].items()) {
        classes.push_back(i.value().get<std::string>());
    }

    for (auto const& i : value["wordCount"].items()) {
        wordCount[i.key()] = std::unordered_map<std::string, int>();
        for (auto const& j : i.value().items()) {
            wordCount[i.key()][j.key()] = j.value().get<int>();
        }
    }

}