#include <arpa/inet.h>
#include <stdexcept>
#include <cstdint>
#include <string>
#include <cstring>
#include <zlib.h>

#include "mnist_reader.h"

using namespace std;

const size_t images_header_size = 4 + 3 * sizeof(uint32_t);
const size_t labels_header_size = 4 + sizeof(uint32_t);

/**
 * Wrapper around a gzFile so that it can be easily closed in case of an exception.
 */
class GzipFile {
public:
    GzipFile(const string &path);
    ~GzipFile();
    int read(uint8_t *buffer, unsigned int buffer_size);
private:
    gzFile m_file_pointer;
};

GzipFile::GzipFile(const string &path) {
    m_file_pointer = gzopen(path.c_str(), "r");
    if (m_file_pointer == nullptr) {
        throw runtime_error("Could not open gzip compressed file");
    }
}

GzipFile::~GzipFile() {
    gzclose(m_file_pointer);
}

int GzipFile::read(uint8_t *buffer, unsigned int buffer_size) {
    return gzread(m_file_pointer, buffer, buffer_size);
}

/**
 * Returns the dimensions of the images contained in the file if the header is valid.
 * Throws an exception otherwise.
 */
static tuple<uint32_t, uint32_t, uint32_t>  parse_images_header(uint8_t header[images_header_size]) {
    /* The idx3 header should start with two zeros, then a magic number that indicates the data type
     * which must be 0x08 for unsigned bytes, then  the number of dimensions which must be 3 because
     * the file contains multiple two-dimensional matrices. */
    if (header[0] != 0 || header[1] != 0 || header[2] != 0x08 || header[3] != 3) {
        throw std::runtime_error("Unexpected magic number in images file header.");
    }
    /* Now there should be three big-endian 32-bit numbers that indicate number of matrices, the
     * number of rows and in each matrix and the number of columns. */
    uint32_t number = 0;
    memcpy(&number, &header[4], sizeof(uint32_t));
    number = htonl(number);
    uint32_t rows = 0;
    memcpy(&rows, &header[8], sizeof(uint32_t));
    rows = htonl(rows);
    uint32_t columns = 0;
    memcpy(&columns, &header[12], sizeof(uint32_t));
    columns = htonl(columns);

    return { number, rows, columns };
}

/**
 * Returns the number of labels. Throws an exception if the header is invalid.
 */
static uint32_t parse_labels_header(uint8_t header[labels_header_size]) {
    /* The idx3 header should start with two zeros, then a magic number that indicates the data type
     * which must be 0x08 for unsigned bytes, then  the number of dimensions which must be 1 because
     * the file contains a single vector of scalars. */
    if (header[0] != 0 || header[1] != 0 || header[2] != 0x08 || header[3] != 1) {
        throw std::runtime_error("Unexpected magic number in labels file header.");
    }
    uint32_t number = 0;
    memcpy(&number, &header[4], sizeof(uint32_t));
    return htonl(number);
}

MnistReader::MnistReader(const std::string &images_path, const std::string &labels_path) {
    GzipFile images_file(images_path);
    GzipFile labels_file(labels_path);

    uint8_t images_header[images_header_size];
    if (images_file.read(images_header, sizeof(images_header)) < sizeof(images_header)) {
        throw runtime_error("Could not read complete idx3 header in images file");
    }
    auto dimensions = parse_images_header(images_header);
    m_images_count = static_cast<size_t>(get<0>(dimensions));
    printf("%lu\n", m_images_count);
    m_image_rows = static_cast<size_t>(get<1>(dimensions));
    printf("%lu\n", m_image_rows);
    m_image_columns = static_cast<size_t>(get<2>(dimensions));
    printf("%lu\n", m_image_columns);

    uint8_t labels_header[labels_header_size];
    if (labels_file.read(labels_header, sizeof(labels_header)) < sizeof(labels_header)) {
        throw runtime_error("Could not read complete idx1 header in labels file");
    }
    uint32_t label_count = parse_labels_header(labels_header);
    if (label_count != m_images_count) {
        printf("%u %lu\n", label_count, m_images_count);
        throw runtime_error("Inconsistent numbers of images and labels.");
    }

    m_image_size = m_image_rows * m_image_columns;
    if (m_images_count > SIZE_MAX / m_image_size) {
        throw runtime_error("Image size too large");
    }
    size_t image_data_size = m_images_count * m_image_size;

    m_image_data.resize(image_data_size);
    if (images_file.read(m_image_data.data(), m_image_data.size()) < image_data_size) {
        throw runtime_error("Image file too short");
    }
    m_label_data.resize(m_images_count);
    if (labels_file.read(m_label_data.data(), m_label_data.size()) < m_images_count) {
        throw runtime_error("Labels file too short");
    }
}

MnistImage MnistReader::getImage(size_t index) const {
    auto data_start = m_image_data.begin() + index * m_image_size;
    auto data_end = data_start + m_image_size;
    return MnistImage(m_image_rows, m_image_columns, data_start, data_end);
}

MnistImage::MnistImage(size_t rows, size_t columns, const vector<uint8_t>::const_iterator &data_start, const vector<uint8_t>::const_iterator &data_end) : m_rows(rows), m_columns(columns) {
    m_data.assign(data_start, data_end);
}

void MnistImage::print() const {
    for (size_t row = 0; row < m_rows; row++) {
        for (size_t col = 0; col < m_columns; col++) {
            uint8_t byte = m_data.at(col * m_rows + row);
            if (byte > 225) {
                printf("#");
            } else if (byte > 180) {
                printf("+");
            } else if (byte > 80) {
                printf(".");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}
