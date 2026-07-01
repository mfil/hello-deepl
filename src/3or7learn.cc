#include <cmath>
#include <cstdio>
#include <random>
#include "mnist_reader.h"
#include "image_processing.h"
#include "visualization.h"

using namespace std;

static random_device r;
static mt19937 rng(r());

static vector<float> makeRandomVector(size_t dim) {
    float max = 1 / sqrt(dim);
    float min = -max;
    uniform_real_distribution<float> distr(min, max);
    vector<float> output(dim);

    for (auto &entry : output) {
        entry = distr(rng);
    }
    return output;
}

template<typename T>
static const T &randomSelect(const vector<T> &vec) {
    uniform_int_distribution<size_t> distr(0, vec.size() - 1);
    size_t index = distr(rng);
    return vec[index];
}

int main() {
    MnistReader reader("../emnist/gzip/emnist-digits-train-images-idx3-ubyte.gz", "../emnist/gzip/emnist-digits-train-labels-idx1-ubyte.gz");

    auto weights = makeRandomVector(28 * 28);
    const float learn_rate = 0.01;
    size_t num_threes = 0;
    size_t num_sevens = 0;
    for (size_t i = 0; i < reader.getImageCount(); i++) {
        auto label = reader.getLabel(i);
        auto image = reader.getImage(i);
        if (label == 3) {
            num_threes += 1;
            if (innerProduct(weights, image) >= 0) {
                scaleVector(image, learn_rate);
                subtractVectors(weights, image);
            }
        } else if (label == 7) {
            num_sevens += 1;
            if (innerProduct(weights, image) < 0) {
                scaleVector(image, learn_rate);
                addVectors(weights, image);
            }
        }
    }
    printf("Trained on %lu threes and %lu sevens.\n", num_threes, num_sevens);
    printDiffToTerminal(weights, 28);

    MnistReader test_reader("../emnist/gzip/emnist-digits-test-images-idx3-ubyte.gz", "../emnist/gzip/emnist-digits-test-labels-idx1-ubyte.gz");

    vector<vector<float>> good_threes{};
    vector<vector<float>> bad_threes{};
    vector<vector<float>> good_sevens{};
    vector<vector<float>> bad_sevens{};
    for (size_t i = 0; i < test_reader.getImageCount(); i++) {
        auto label = test_reader.getLabel(i);
        auto image = test_reader.getImage(i);
        if (label == 3) {
            if (innerProduct(weights, image) < 0) {
                good_threes.push_back(std::move(image));
            } else {
                bad_threes.push_back(std::move(image));
            }
        } else if (label == 7) {
            if (innerProduct(weights, image) >= 0) {
                good_sevens.push_back(std::move(image));
            } else {
                bad_sevens.push_back(std::move(image));
            }
        }
    }
    size_t total_threes = good_threes.size() + bad_threes.size();
    size_t total_sevens = good_sevens.size() + bad_sevens.size();
    size_t total = total_threes + total_sevens;
    printf("Loaded %lu testing images, %lu threes and %lu sevens.\n", total, total_threes, total_sevens);
    printf("Correctly predicted %lu out of %lu threes, %.02f%%\n", good_threes.size(), total_threes, 100.0 * good_threes.size() / total_threes);
    printf("Correctly predicted %lu out of %lu sevens, %.02f%%\n", good_sevens.size(), total_sevens, 100.0 * good_sevens.size() / total_sevens);
    printf("Total: %lu out of %lu, %.02f%%\n", good_threes.size() + good_sevens.size(), total, 100.0 * (good_threes.size() + good_sevens.size()) / total);

    printf("Random correctly classified 3:");
    if (good_threes.size() > 0) {
        printImageToTerminal(randomSelect(good_threes), 28);
    } else {
        printf("\nNo example available.");
    }

    printf("Random incorrectly classified 3:");
    if (bad_threes.size() > 0) {
        printImageToTerminal(randomSelect(bad_threes), 28);
    } else {
        printf("\nNo example available.");
    }

    printf("Random correctly classified 7:");
    if (good_sevens.size() > 0) {
        printImageToTerminal(randomSelect(good_sevens), 28);
    } else {
        printf("\nNo example available.");
    }

    printf("Random incorrectly classified 7:");
    if (bad_sevens.size() > 0) {
        printImageToTerminal(randomSelect(bad_sevens), 28);
    } else {
        printf("\nNo example available.");
    }
    return 0;
}
