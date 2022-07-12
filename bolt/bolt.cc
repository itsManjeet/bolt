#include "bolt.hh"
using namespace bolt;

#include <float.h>

#include <iostream>

Bolt::ptr Bolt::create(std::string filepath) {
  return std::make_shared<Bolt>(YAML::LoadFile(filepath));
}

Bolt::Bolt(YAML::Node const& node) {
  mId = node["id"].as<std::string>();

  mLogics = logics::Logic::logics(node);
  mPlugins = plugins::Plugin::plugins(node);
  mBackend = backend::Backend::create(node["backend"]);

  for (auto const& logic : mLogics) {
    auto trainable = std::dynamic_pointer_cast<logics::Trainable>(logic);
    if (trainable == nullptr) {
      continue;
    }

    if (!trainable->train()) {
      std::cerr << "Failed to load model: " << logic->error() << std::endl;
      continue;
    }
  }
}

std::tuple<float, std::string> Bolt::predict(std::string input) {
  float highScore = 0.0;
  std::string highIntention = "unknown";

  for (auto const& i : mLogics) {
    auto [score, prediction] = i->predict(input);
    if (score > highScore) {
      highScore = score;
      highIntention = prediction;
    }
  }
  auto iter = mPlugins.find(highIntention);
  if (iter == mPlugins.end()) {
    return {FLT_MAX, "sorry, i have no idea about that"};
  }
  std::cerr << "intension : " << highIntention << std::endl;
  std::cerr << "score     : " << highScore << std::endl;
  auto response = iter->second->respond(highIntention, input);
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