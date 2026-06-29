#include <iostream>
#include "mnist_reader.h"

using namespace std;

int main() {
    MnistReader reader("../emnist/gzip/emnist-digits-train-images-idx3-ubyte.gz", "../emnist/gzip/emnist-digits-train-labels-idx1-ubyte.gz");

    cout << static_cast<uint32_t>(reader.getLabel(0)) << endl;
    reader.getImage(0).print();
    cout << static_cast<uint32_t>(reader.getLabel(1)) << endl;
    reader.getImage(1).print();
    cout << static_cast<uint32_t>(reader.getLabel(2)) << endl;
    reader.getImage(2).print();
    cout << static_cast<uint32_t>(reader.getLabel(20)) << endl;
    reader.getImage(20).print();
    return 0;
}
