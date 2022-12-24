#include "Classifier.hxx"
using namespace bolt::classifier;

#include <fstream>
#include <map>
#include <vector>

void Classifier::train(std::string filepath) {
    std::ifstream reader(filepath);
    if (!reader.good()) {
        throw std::runtime_error("Failed to open trainning file '" + filepath +
                                 "'");
    }

    std::string line;

    std::map<std::string, std::vector<std::string>> trainingData;
    std::string lastIntension = "none";

    while (std::getline(reader, line)) {
        if (line.size() == 0) {
            continue;
        }
        if (line[0] == '-') {
            trainingData[lastIntension].push_back(line.substr(1));
        } else {
            lastIntension = line;
            trainingData[line] = std::vector<std::string>();
        }
    }

    for (auto const& data : trainingData) {
        train(data.second.begin(), data.second.end(), data.first);
    }
}