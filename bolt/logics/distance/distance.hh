#ifndef BOLT_LOGICS_DISTANCE_HH
#define BOLT_LOGICS_DISTANCE_HH

#include "../logic.hh"
#define DEFAULT_DISTANCE_MODEL_PATH "/var/cache/bolt/distance.model"

namespace bolt::logics {
class Distance : public Logic, public Trainable {
 private:
  std::string mModelPath;
  std::map<std::string, std::string> mCache;

 public:
  Distance(YAML::Node const& node);

  bool train(
      std::vector<std::pair<std::string, std::string>> const& trainingData);

  std::tuple<float, std::string> predict(std::string input);
};
}  // namespace bolt::logics

#endif