#include <vector>
#include <cstdio>

using namespace std;

void printImageToTerminal(const vector<float> &image, size_t num_columns) {
    size_t num_rows = image.size() / num_columns;
    for (size_t row = 0; row < num_rows; row++) {
        for (size_t col = 0; col < num_columns; col++) {
            float pixel_brightness = image[col * num_rows + row];
            if (pixel_brightness > 0.9) {
                printf("#");
            } else if (pixel_brightness > 0.6) {
                printf("+");
            } else if (pixel_brightness > 0.3) {
                printf(".");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}
