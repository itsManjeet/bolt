#include <algorithm>
#include <config.hxx>
#include <fstream>
#include <map>

#include "../../src/Plugin.hxx"

float levenshteinDistance(std::string const& sent1, std::string const& sent2) {
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

    return distance[minSize];
}

static std::map<std::string, std::vector<std::string>> knownMemory;

PLUGIN_FUNCTION(main) {
    std::ifstream reader(BOLT_DATA_DIR "/responses.txt");
    if (!reader.good()) {
        os << "Sorry i have no idea about that";
        return;
    }

    if (knownMemory.size() == 0) {
        std::string line;
        std::string lastResponse;
        std::vector<std::string> responses;
        while (std::getline(reader, line)) {
            if (line.size() == 0) {
                continue;
            }

            if (line[0] == '-') {
                knownMemory[lastResponse].push_back(line.substr(1));
                continue;
            }

            std::transform(line.begin(), line.end(), line.begin(), tolower);
            lastResponse = line;
            if (knownMemory.find(lastResponse) == knownMemory.end()) {
                knownMemory[lastResponse] = std::vector<std::string>();
            }
        }
    }

    float miminumDistance = ctxt->rawSentence.length();
    std::vector<std::string> selectedResponses = {
        "sorry i have no idea about that"};

    for (auto const& r : knownMemory) {
        auto distance = levenshteinDistance(r.first, ctxt->rawSentence);
        if (distance < miminumDistance) {
            miminumDistance = distance;
            selectedResponses = r.second;
            if (distance == 0) {
                break;
            }
        }
    }

    os << selectedResponses.back();
}