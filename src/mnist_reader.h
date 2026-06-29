#include <string>
#include <cstdint>
#include <vector>
#include "zlib.h"

using namespace std;

class MnistReader {
public:
    MnistReader(const std::string &images_path, const std::string &labels_path);
    vector<float> getImage(size_t index) const;
    uint8_t getLabel(size_t index) const { return m_label_data.at(index); };
private:
    size_t m_images_count;
    size_t m_image_rows;
    size_t m_image_columns;
    size_t m_image_size;
    vector<uint8_t> m_image_data;
    vector<uint8_t> m_label_data;
};
