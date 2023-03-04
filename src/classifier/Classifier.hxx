#ifndef __BOLT_CLASSIFIER_HXX__
#define __BOLT_CLASSIFIER_HXX__

#include <string>
#include <tuple>
#include <vector>

namespace bolt::classifier {
class Classifier {
   protected:
    virtual void train(std::vector<std::string>::const_iterator text_begin,
                       std::vector<std::string>::const_iterator text_end,
                       std::vector<std::string>::const_iterator label_begin,
                       std::vector<std::string>::const_iterator label_end) = 0;

   public:
    virtual ~Classifier() {}
    virtual std::vector<std::tuple<std::string, double>> classify(
        std::string const& query) = 0;

    void train(std::string filepath);

    virtual void save(std::string outputfile) = 0;

    virtual void load(std::string modelfile) = 0;
};
}  // namespace bolt::classifier

#endif