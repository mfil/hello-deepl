#include <iostream>
#include "mnist_reader.h"
#include "visualization.h"

using namespace std;

int main() {
    MnistReader reader("../emnist/gzip/emnist-digits-train-images-idx3-ubyte.gz", "../emnist/gzip/emnist-digits-train-labels-idx1-ubyte.gz");

    cout << static_cast<uint32_t>(reader.getLabel(0)) << endl;
    print_image_to_terminal(reader.getImage(0), 28);
    cout << static_cast<uint32_t>(reader.getLabel(1)) << endl;
    print_image_to_terminal(reader.getImage(1), 28);
    cout << static_cast<uint32_t>(reader.getLabel(2)) << endl;
    print_image_to_terminal(reader.getImage(2), 28);
    cout << static_cast<uint32_t>(reader.getLabel(20)) << endl;
    print_image_to_terminal(reader.getImage(20), 28);
    return 0;
}
