#include "Model.hxx"

using namespace bolt;

#include <algorithm>
#include <fstream>
#include <iostream>
#include "json.hpp"

using namespace nlohmann;

#include "nlp.hxx"

void Model::train(std::vector<std::string>::const_iterator text_begin,
                  std::vector<std::string>::const_iterator text_end,
                  std::vector<std::string>::const_iterator label_begin,
                  std::vector<std::string>::const_iterator label_end,
                  std::map<std::string, std::vector<std::string>>::const_iterator responses_begin,
                  std::map<std::string, std::vector<std::string>>::const_iterator responses_end) {
    for (auto text = text_begin; text != text_end; text++) {
        auto label = *label_begin++;
        if (classCount.find(label) == classCount.end()) {
            classes.push_back(label);
            classCount[label] = 0;
        }
        classCount[label]++;

        for (auto const &word: nlp::tokenize(*text)) {
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

    for (auto i = responses_begin; i != responses_end; i++) {
        auto iter = responses.find(i->first);
        if (iter == responses.end()) {
            responses[i->first] = i->second;
            continue;
        }
        iter->second.insert(iter->second.end(), i->second.begin(), i->second.end());
    }
}

std::vector<std::pair<std::string, float>> Model::classify(
        std::string const &query) {
    std::vector<std::pair<std::string, float>> solutions;
    float max_probability = 0.0;
    float sum_probability = 0.0;
    for (auto const &intension: classes) {
        float probability = classCount[intension] * (1.0 / classCount.size());
        for (auto const &word: nlp::tokenize(query)) {
            if (wordCount[intension].find(word) != wordCount[intension].end()) {
                probability *=
                        (wordCount[intension][word] + 1.0) / (classCount[intension] + wordCount[intension].size());
            } else {
                probability *= 1.0 / (classCount[intension] + wordCount[intension].size());
            }
        }
        sum_probability += probability;
        if (probability > max_probability) {
            max_probability = probability;
        }
        solutions.push_back({intension, probability});
    }
    std::sort(solutions.begin(), solutions.end(), [](auto const& lhs, auto const &rhs) -> bool {
        return lhs.second > rhs.second;
    });

    return solutions;
}

void Model::save(std::string outputfile) {
    nlohmann::json json = {
            {"vocabulary", vocabulary},
            {"classCount", classCount},
            {"wordCount",  wordCount},
            {"classes",    classes},
            {"responses",  responses},
    };

    std::ofstream writer(outputfile, std::ios_base::binary);
    writer << json.dump() << std::flush;
}

void Model::load(std::string modelfile) {
    std::ifstream reader(modelfile);
    if (!reader.good()) {
        throw std::runtime_error("failed to read model file '" + modelfile +
                                 "'");
    }
    auto value = json::parse(reader);

    for (auto const &i: value["vocabulary"].items()) {
        vocabulary[i.key()] = i.value().get<int>();
    }

    for (auto const &i: value["classCount"].items()) {
        classCount[i.key()] = i.value().get<int>();
    }

    for (auto const &i: value["classes"].items()) {
        classes.push_back(i.value().get<std::string>());
    }

    for (auto const &i: value["wordCount"].items()) {
        wordCount[i.key()] = std::unordered_map<std::string, int>();
        for (auto const &j: i.value().items()) {
            wordCount[i.key()][j.key()] = j.value().get<int>();
        }
    }

    for (auto const &i: value["responses"].items()) {
        responses[i.key()] = std::vector<std::string>();
        for (auto const &j: i.value().items()) {
            responses[i.key()].push_back(j.value().get<std::string>());
        }
    }

}

void Model::train(const std::string &filepath) {
    std::ifstream reader(filepath);
    if (!reader.good()) {
        throw std::runtime_error("Failed to read file '" + filepath + "'");
    }

    bool isResponsesStart = false;

    std::string line;
    std::vector<std::string> text;
    std::vector<std::string> label;
    std::map<std::string, std::vector<std::string>> responses;
    std::string HEAD = "unknown";

    while (std::getline(reader, line)) {
        line = nlp::trim(line);
        if (line.size() == 0) continue;
        if (line == "%%") {
            isResponsesStart = true;
            continue;
        }

        if (line[0] != '-') {
            HEAD = line;
            if (isResponsesStart) {
                responses[HEAD] = std::vector<std::string>();
            }
        } else {
            line = nlp::trim(line.substr(1));
            if (isResponsesStart) {
                responses[HEAD].push_back(line);
            } else {
                text.push_back(line);
                label.push_back(HEAD);
            }
        }
    }

    return train(text.begin(), text.end(),
                 label.begin(), label.end(),
                 responses.begin(), responses.end());
}

float Model::levenshteinDistance(std::string const &sent1, std::string const &sent2) {
    if (sent1.size() > sent2.size()) {
        return levenshteinDistance(sent2, sent1);
    }

    auto minSize = sent1.size();
    auto maxSize = sent2.size();

    std::vector<int> distance(minSize + 1);
    for (int i = 0; i <= minSize; i++) {
        distance[i] = i;
    }

    for (int j = 1; j <= maxSize; j++) {
        int previousDiagonal = distance[0], previousDiagonalSaved;
        ++distance[0];

        for (int i = 1; i <= minSize; i++) {
            previousDiagonalSaved = distance[i];
            if (sent1[i - 1] == sent2[j - 1]) {
                distance[i] = previousDiagonal;
            } else {
                distance[i] = std::min(std::min(distance[i - 1], distance[i]),
                                       previousDiagonal) +
                              1;
            }

            previousDiagonal = previousDiagonalSaved;
        }
    }

    return 1.0 - ((float)distance[minSize] / (float)maxSize);
}


std::pair<std::vector<std::string>, float> Model::fuzzySearch(const std::string &query) {
    std::vector<std::string> res = {
            "Sorry i have no idea about that",
    };
    float bestScore = 0.0;

    for (auto const &i: responses) {
        auto score = levenshteinDistance(i.first, query);
        if (score > bestScore) {
            bestScore = score;
            res = i.second;
        }
    }

    return {res, bestScore};
}
