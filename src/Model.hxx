#ifndef __BOLT_CLASSIFIER_NAIVE_BAYES_HXX__
#define __BOLT_CLASSIFIER_NAIVE_BAYES_HXX__

#include <map>
#include <tuple>
#include <unordered_map>
#include <vector>
#include <string>

namespace bolt {
class Model {
   private:
    std::unordered_map<std::string, int> vocabulary;
    std::unordered_map<std::string, int> classCount;
    std::unordered_map<std::string, std::unordered_map<std::string, int>> wordCount;
    std::vector<std::string> classes;
    std::map<std::string, std::vector<std::string>> responses;

    float levenshteinDistance(std::string const& s1, std::string const& s2);

   public:
    Model() {}
    virtual ~Model() {}
    std::vector<std::pair<std::string, float>> classify(
        std::string const& query);

    std::pair<std::vector<std::string>, float> fuzzySearch(std::string const& query);

    void train(std::vector<std::string>::const_iterator text_begin,
               std::vector<std::string>::const_iterator text_end,
               std::vector<std::string>::const_iterator label_begin,
               std::vector<std::string>::const_iterator label_end,
               std::map<std::string, std::vector<std::string>>::const_iterator responses_begin,
               std::map<std::string, std::vector<std::string>>::const_iterator responses_end);

    void train(std::string const& filepath);


    void save(std::string outputfile);

    void load(std::string modelfile);
};
}  // namespace bolt::classifier

#endif