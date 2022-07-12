#ifndef BOLT_LOGICS_HH
#define BOLT_LOGICS_HH

#include <yaml-cpp/yaml.h>

#include <string>
#include <tuple>
namespace bolt::logics {
/**
 * @brief Provide bolt interface for logic
 */
class Logic {
 public:
  using ptr = std::shared_ptr<Logic>;
  using List = std::vector<Logic::ptr>;

 protected:
  std::string mError;  //!< Holds the error message
  YAML::Node mNode;    //!< Holds the configuration

 public:
  Logic(YAML::Node const& node) : mNode(node) {}

  /**
   * @brief Predict the response of specified input
   * @param input input message
   * @return std::tuple<float, std::string> intension with score
   */
  virtual std::tuple<float, std::string> predict(std::string input) = 0;

  /**
   * @brief Reports the error message set by functions
   * @return std::string const& error message
   */
  std::string const& error() const { return mError; }

  static Logic::List logics(YAML::Node const& node);
};

/**
 * @brief Trainable provide interface for trainable logics
 */
class Trainable {
 public:
  /**
   * @brief Train the logic model
   * @return true on success
   * @return false on failure with error message set
   */
  virtual bool train() = 0;
};
}  // namespace bolt::logics

#endif