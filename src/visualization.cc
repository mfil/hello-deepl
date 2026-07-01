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

void printDiffToTerminal(const vector<float> &diff, size_t num_columns) {
    const char blue[] = "\033[44m";
    const char red[] = "\033[41m";
    const char reset[] = "\033[0m";

    size_t num_rows = diff.size() / num_columns;
    for (size_t row = 0; row < num_rows; row++) {
        for (size_t col = 0; col < num_columns; col++) {
            float diff_value = diff[col * num_rows + row];
            if (diff_value > 0) {
                printf(blue);
            } else if (diff_value < 0) {
                printf(red);
                diff_value = -diff_value;
            }
            if (diff_value > 0.9) {
                printf("#");
            } else if (diff_value > 0.6) {
                printf("X");
            } else if (diff_value > 0.3) {
                printf("+");
            } else if (diff_value > 0.1) {
                printf(".");
            } else {
                printf(" ");
            }
            printf(reset);
        }
        printf("\n");
    }
}
