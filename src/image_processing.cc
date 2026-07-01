#include <algorithm>
#include <stdexcept>
#include "image_processing.h"

using namespace std;

void addVectors(vector<float> &a, const vector<float> &b) {
    if (a.size() != b.size()) {
        throw runtime_error("vectors have different dimension");
    }
    for (size_t i = 0; i < a.size(); i++) {
        a[i] += b[i];
    }
}

void subtractVectors(vector<float> &a, const vector<float> &b) {
    if (a.size() != b.size()) {
        throw runtime_error("vectors have different dimension");
    }
    for (size_t i = 0; i < a.size(); i++) {
        a[i] -= b[i];
    }
}

void scaleVector(vector<float> &a, float factor) {
    for (float &x : a) {
        x *= factor;
    }
}

vector<float> average(const vector<vector<float>> &images) {
    if (images.size() == 0) {
        return vector<float>(0);
    }
    vector<float> result(images[0].size());
    for (auto &&image : images) {
        addVectors(result, image);
    }
    for_each(result.begin(), result.end(), [&](float &x) { x /= images.size(); });
    return result;
}

float innerProduct(const vector<float> &a, const vector<float> &b) {
    if (a.size() != b.size()) {
        throw runtime_error("vectors have different dimension");
    }
    float result = 0;
    for (size_t i = 0; i < a.size(); i++) {
        result += a[i] * b[i];
    }
    return result;
}
