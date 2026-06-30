#include <cstdio>
#include "mnist_reader.h"
#include "image_processing.h"
#include "visualization.h"

using namespace std;

int main() {
    MnistReader reader("../emnist/gzip/emnist-digits-train-images-idx3-ubyte.gz", "../emnist/gzip/emnist-digits-train-labels-idx1-ubyte.gz");

    vector<vector<float>> threes(0);
    vector<vector<float>> sevens(0);
    for (size_t i = 0; i < reader.getImageCount(); i++) {
        auto label = reader.getLabel(i);
        if (label == 3) {
            threes.push_back(reader.getImage(i));
        } else if (label == 7) {
            sevens.push_back(reader.getImage(i));
        }
    }
    printf("Loaded %lu training images, %lu threes and %lu sevens.\n", threes.size() + sevens.size(), threes.size(), sevens.size());

    auto average_three = average(threes);
    auto average_seven = average(sevens);
    printf("Average three:\n");
    printImageToTerminal(average_three, 28);
    printf("Average seven:\n");
    printImageToTerminal(average_seven, 28);

    auto diff = average_seven;
    subtractVectors(diff, average_three);

    float avg_three_score = 0;
    for (auto three : threes) {
        avg_three_score += inner_product(diff, three);
    }
    avg_three_score /= threes.size();
    printf("avg. three: %f\n", avg_three_score);

    float avg_seven_score = 0;
    for (auto seven : sevens) {
        avg_seven_score += inner_product(diff, seven);
    }
    avg_seven_score /= sevens.size();
    printf("avg. seven: %f\n", avg_seven_score);

    float bias = (avg_three_score + avg_seven_score) / 2;
    printf("bias = %f\n", bias);

    MnistReader test_reader("../emnist/gzip/emnist-digits-test-images-idx3-ubyte.gz", "../emnist/gzip/emnist-digits-test-labels-idx1-ubyte.gz");

    vector<vector<float>> test_threes(0);
    vector<vector<float>> test_sevens(0);
    for (size_t i = 0; i < test_reader.getImageCount(); i++) {
        auto label = test_reader.getLabel(i);
        if (label == 3) {
            test_threes.push_back(test_reader.getImage(i));
        } else if (label == 7) {
            test_sevens.push_back(test_reader.getImage(i));
        }
    }
    size_t total = test_threes.size() + test_sevens.size();
    printf("Loaded %lu testing images, %lu threes and %lu sevens.\n", total, test_threes.size(), test_sevens.size());

    size_t successes = 0;
    for (auto &three : test_threes) {
        if (inner_product(diff, three) < bias) {
            successes += 1;
        }
    }
    for (auto &seven : test_sevens) {
        if (inner_product(diff, seven) >= bias) {
            successes += 1;
        }
    }
    printf("Predicted %lu out ouf %lu images, %.02f%%\n", successes, total, (100.0 * successes) / total);
    return 0;
}
