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

    std::vector<std::string> intensionData;
    std::vector<std::string> trainingData;
    std::string lastIntension = "none";

    while (std::getline(reader, line)) {
        if (line.size() == 0) {
            continue;
        }
        if (line[0] == '-') {
            trainingData.push_back(line.substr(1));
            intensionData.push_back(lastIntension);
        } else {
            lastIntension = line;
        }
    }

    train(trainingData.begin(), trainingData.end(),
          intensionData.begin(), intensionData.end());
}