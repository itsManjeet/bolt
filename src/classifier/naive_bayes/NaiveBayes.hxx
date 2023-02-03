#ifndef __BOLT_CLASSIFIER_NAIVE_BAYES_HXX__
#define __BOLT_CLASSIFIER_NAIVE_BAYES_HXX__

#include <map>
#include <tuple>
#include <vector>

#include "../Classifier.hxx"

namespace bolt::classifier {
class NaiveBayes : public Classifier {
   private:
    std::map<std::string, std::vector<std::string>> classWords;
    std::map<std::string, float> corpusWords;

    float calculate(std::string const& sentence, std::string const& intension);

   protected:
    void train(std::vector<std::string>::const_iterator iter,
               std::vector<std::string>::const_iterator end,
               std::string intension);

   public:
    NaiveBayes() {}
    virtual ~NaiveBayes() {}
    std::vector<std::tuple<std::string, float>> classify(
        std::string const& query);

    void save(std::string outputfile);

    void load(std::string modelfile);
};
}  // namespace bolt::classifier

#endif