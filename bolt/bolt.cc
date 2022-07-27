#include "bolt.hh"
using namespace bolt;

#include <float.h>

#include <algorithm>
#include <fstream>
#include <iostream>

#include "Debug.hh"

Bolt::ptr Bolt::create(std::string filepath) {
  LOG("BOLT", "LOADING CONFIGURATION '" << filepath << "'");
  return std::make_shared<Bolt>(YAML::LoadFile(filepath));
}

Bolt::Bolt(YAML::Node const& node) {
  mId = node["id"].as<std::string>();
  LOG("BOLT", "ID = '" << mId << "'");

  LOG("BOLT", "GETTING LOGICS");
  mLogics = logics::Logic::logics(node);
  LOG("BOLT", "FOUND " << mLogics.size() << " LOGICS");

  LOG("BOLT", "GETTING PLUGINS");
  mPlugins = plugins::Plugin::plugins(node);
  LOG("BOLT", "FOUND " << mPlugins.size() << " PLUGINS");

  LOG("BOLT", "SETTING BACKEND");
  mBackend = backend::Backend::create(node["backend"]);
}

class Comparator {
 public:
  bool operator()(std::tuple<float, std::string> const& lhs,
                  std::tuple<float, std::string> const& rhs) {
    return std::get<0>(lhs) < std::get<0>(rhs);
  }
};

bool Bolt::train(std::string logicId, std::string trainingFilePath) {
  auto iter = std::find_if(
      mLogics.begin(), mLogics.end(),
      [&logicId](std::pair<std::string, logics::Logic::ptr> const& l) -> bool {
        return logicId == l.first;
      });

  if (iter == mLogics.end()) {
    mError = "No logic found with id '" + logicId + "'";
    return false;
  }

  auto trainable = std::dynamic_pointer_cast<logics::Trainable>(iter->second);
  if (trainable == nullptr) {
    mError = "Logic '" + logicId + "' is not trainable";
    return false;
  }

  std::ifstream trainingFile(trainingFilePath);
  if (!trainingFile.good()) {
    mError = "Failed to load training file '" + trainingFilePath + "'";
    return false;
  }
  std::vector<std::pair<std::string, std::string>> trainingData;
  std::string line;
  while (std::getline(trainingFile, line, '\n')) {
    size_t idx = line.find_first_of(':');
    if (idx == std::string::npos) {
      continue;
    }
    trainingData.push_back({line.substr(0, idx), line.substr(idx + 1)});
  }
  LOG("BOLT", "TRAINING DATA SIZE = " << trainingData.size());
  return trainable->train(trainingData);
}

std::string Bolt::respond(std::string intention, std::string input) {
  std::string plugin_id = intention;
  std::string data = "";

  if (intention.size() && intention[0] == '@') {
    size_t idx = intention.find_first_of('#');
    plugin_id = intention.substr(1, idx);
    data = intention.substr(idx + 1);
  }
  LOG("BOLT", "RESPONSE PLUGIN ID '" << plugin_id << "'");
  LOG("BOLT", "RESPONSE DATA '" << data << "'");

  auto iter = mPlugins.find(plugin_id);
  if (iter == mPlugins.end()) {
    return intention;
  }
  return iter->second->respond(intention, data);
}

std::tuple<float, std::string> Bolt::predict(std::string input) {
  float highScore = 0.0;
  std::string highIntention = "unknown";

  LOG("BOLT", "PREDICTION FOR '" << input << "'");
  for (auto const& i : mLogics) {
    auto [score, prediction] = i.second->predict(input);
    if (score > highScore) {
      highScore = score;
      highIntention = prediction;
    }
  }
  LOG("BOLT", "PREDICTION SCORE = '" << highScore << "'");
  LOG("BOLT", "PREDICTION DATA = '" << highIntention << "'");

  std::string response = respond(highIntention, input);

  return {highScore, response};
}

bool Bolt::start() {
  if (mBackend == nullptr) {
    mError = "no backend found";
    return false;
  }

  if (!mBackend->start(this)) {
    mError = mBackend->error();
    return false;
  }
  return true;
}