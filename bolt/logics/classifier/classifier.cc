#include "classifier.hh"
using namespace bolt::logics;

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>

Classifier::Classifier(YAML::Node const& node) : Logic(node) {}

bool Classifier::train(
    std::vector<std::pair<std::string, std::string>> const& trainingData) {
  std::string model_path = mNode["model-path"].as<std::string>();
  for (auto const& i : trainingData) {
    std::string intention = i.second;
    std::string sentence = i.first;
    
    std::stringstream words(sentence);
    std::string word;
    while (words >> word) {
      auto iter = mCorpusWords.find(word);
      if (iter == mCorpusWords.end())
        mCorpusWords[word] = 1.0;
      else
        iter->second += 1.0;

      if (mClassWords.find(intention) == mClassWords.end()) {
        mClassWords[intention] = std::vector<std::string>();
      }
      mClassWords[intention].push_back(word);
    }
  }

  return true;
}

float Classifier::calculate(std::string sentence, std::string intension,
                            bool show_details) {
  float score = 0.0;
  auto iter = mClassWords.find(intension);
  if (iter == mClassWords.end()) {
    return score;
  }

  std::stringstream ss(sentence);
  std::string word;
  while (ss >> word) {
    if (std::find(iter->second.begin(), iter->second.end(), word) !=
        iter->second.end()) {
      score += (1.0 / mCorpusWords[word]);
    }

    if (show_details) {
      std::cerr << " match: " << word << ", " << (1.0 / mCorpusWords[word])
                << ", " << mCorpusWords[word] << std::endl;
    }
  }
  return score;
}

std::tuple<float, std::string> Classifier::predict(std::string input) {
  std::string targetIntension;
  float targetScore = 0.0;
  for (auto const& intension : mClassWords) {
    std::cerr << "intension: " << intension.first << std::endl;
    float score = calculate(input, intension.first, true);
    std::cerr << " score: " << score << std::endl;
    if (score > targetScore) {
      targetScore = score;
      targetIntension = intension.first;
    }
  }
  return {targetScore, targetIntension};
}