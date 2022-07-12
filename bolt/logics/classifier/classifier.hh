#ifndef BOLT_LOGICS_CLASSIFIER_HH
#define BOLT_LOGICS_CLASSIFIER_HH

#include "../logic.hh"

namespace bolt::logics {
/**
 * @brief Naive bayes classifier implementation
 *
 */
class Classifier : public Logic, public Trainable {
 private:
  std::map<std::string, std::vector<std::string>> mClassWords;
  std::map<std::string, float> mCorpusWords;

  float calculate(std::string sentence, std::string intension,
                  bool show_details = false);

 public:
  Classifier(YAML::Node const& node);

  bool train();

  std::tuple<float, std::string> predict(std::string input);
};
}  // namespace bolt::logics

#endif