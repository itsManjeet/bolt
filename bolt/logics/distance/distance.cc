#include "distance.hh"
using namespace bolt::logics;
#include <float.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>

template <typename T>
typename T::size_type __distance__(const T& source, const T& target) {
  if (source.size() > target.size()) {
    return __distance__(target, source);
  }

  using TSizeType = typename T::size_type;
  const TSizeType min_size = source.size(), max_size = target.size();
  std::vector<TSizeType> lev_dist(min_size + 1);

  for (TSizeType i = 0; i <= min_size; ++i) {
    lev_dist[i] = i;
  }

  for (TSizeType j = 1; j <= max_size; ++j) {
    TSizeType previous_diagonal = lev_dist[0], previous_diagonal_save;
    ++lev_dist[0];

    for (TSizeType i = 1; i <= min_size; ++i) {
      previous_diagonal_save = lev_dist[i];
      if (source[i - 1] == target[j - 1]) {
        lev_dist[i] = previous_diagonal;
      } else {
        lev_dist[i] = std::min(std::min(lev_dist[i - 1], lev_dist[i]),
                               previous_diagonal) +
                      1;
      }
      previous_diagonal = previous_diagonal_save;
    }
  }

  return lev_dist[min_size];
}

Distance::Distance(YAML::Node const& node)
    : Logic(node), mModelPath(DEFAULT_DISTANCE_MODEL_PATH) {
  if (node["model-path"]) {
    mModelPath = node["model-path"].as<std::string>();
  }
  if (std::filesystem::exists(mModelPath)) {
    std::cout << "[DISTANCE]: LOADING MODEL '" << mModelPath << "'"
              << std::endl;
    std::ifstream infile(mModelPath);
    std::string line;
    while (std::getline(infile, line, '\n')) {
      std::cout << "[DISTANCE]: GOT '" << line << "'" << std::endl;
      size_t idx = line.find_first_of(':');
      if (idx == std::string::npos) {
        continue;
      }
      mCache[line.substr(0, idx)] = line.substr(idx + 1);
    }
    std::cout << "[DISTANCE]: MODEL SIZE = " << mCache.size() << std::endl;
  }
}

bool Distance::train(
    std::vector<std::pair<std::string, std::string>> const& trainingData) {
  std::ofstream model(mModelPath);
  std::cout << "[DISTANCE]: TRAINING MODEL" << std::endl;
  for (auto const& i : trainingData) {
    auto iter = mCache.find(i.first);
    if (iter == mCache.end()) {
      std::cout << "[DISTANCE]: NEW DATA '" << i.first << "'" << std::endl;
      mCache[i.first] = i.second;
      model << i.first << ':' << i.second << std::endl;
    }
  }

  model.close();
  std::cout << "[DISTANCE]: MODEL SIZE = " << mCache.size() << std::endl;
  return true;
}

std::tuple<float, std::string> Distance::predict(std::string input) {
  float max_score = FLT_MIN;
  std::string response = "sorry i have no idea about that";

  std::cout << "[DISTANCE]: PREDICTION '" << input << '\'' << std::endl;

  for (auto const& i : mCache) {
    auto distance = __distance__(input, i.first);
    float score =
        (float((std::max(input.length(), i.first.length())) - distance) /
         (std::max(input.length(), i.first.length()))) *
        100.0f;

    std::cout << "[DISTANCE]: FOUND  RESPONSE '" << i.second << "' with score "
              << score << std::endl;
    if (max_score < score) {
      max_score = score;
      response = i.second;
      if (max_score == 100.0f) {
        break;
      }
    }
  }

  return {max_score, response};
}