#ifndef __BOLT_CLASSIFIER_NAIVE_BAYES_HXX__
#define __BOLT_CLASSIFIER_NAIVE_BAYES_HXX__

#include <map>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "../Classifier.hxx"

namespace bolt::classifier {
class NaiveBayes : public Classifier {
   private:
    std::unordered_map<std::string, int> vocabulary;
    std::unordered_map<std::string, int> classCount;
    std::unordered_map<std::string, std::unordered_map<std::string, int>> wordCount;
    std::vector<std::string> classes;

   protected:
    void train(std::vector<std::string>::const_iterator text_begin,
               std::vector<std::string>::const_iterator text_end,
               std::vector<std::string>::const_iterator label_begin,
               std::vector<std::string>::const_iterator label_end);

   public:
    NaiveBayes() {}
    virtual ~NaiveBayes() {}
    std::vector<std::tuple<std::string, double>> classify(
        std::string const& query);

    void save(std::string outputfile);

    void load(std::string modelfile);
};
}  // namespace bolt::classifier

#endif