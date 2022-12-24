#include "../src/classifier/naive_bayes/NaiveBayes.hxx"
using namespace bolt::classifier;

#include <gtest/gtest.h>

TEST(NaiveBayesTest, trainner_test) {
    std::unique_ptr<Classifier> naiveBayes = std::make_unique<NaiveBayes>();
    naiveBayes->train(DATA_PATH "/training.txt");

    ASSERT_EQ(std::get<0>(naiveBayes->classify("switch to devel")), "ostree::switch");

    naiveBayes->save(CACHE_PATH "/model.json");
    naiveBayes->load(CACHE_PATH "/model.json");

    ASSERT_EQ(std::get<0>(naiveBayes->classify("switch to devel")), "ostree::switch");
}