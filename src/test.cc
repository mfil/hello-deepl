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

    auto average_three = average(threes);
    auto average_seven = average(sevens);
    printf("Average three:\n");
    printImageToTerminal(average_three, 28);
    printf("Average seven:\n");
    printImageToTerminal(average_seven, 28);
    return 0;
}
