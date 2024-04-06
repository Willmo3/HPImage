#ifndef HPIMAGE_HPIMAGE_H
#define HPIMAGE_HPIMAGE_H

/**
 * HPImage. An image procesing library for use with the HPCarver program.
 * It is a simple, object-oriented utility for loading PPM files into memory.
 *
 * This is effectively an OO-version of the C-based netpbm library by William Lovo and Dr. Mike Lam
 * Author: Will Morris.
 *
 * Refer to https://davis.lbl.gov/Manuals/NETPBM/doc/ppm.html
 */
#include <cstdint>
#include <cstddef>

namespace hpimage {

// When writing, only put at most 60 pixels on a given line in the actual file.
const uint16_t MAX_PIXELS_PER_LINE = 60;

// The PPM specification allows for RGB values to potentially take more than 8 bytes.
typedef uint16_t rgb_t;

// For the purposes of hpimage, a pixel is just a set of RGB values.
// We will not be operating on transparent images.
struct pixel {
    rgb_t red;
    rgb_t green;
    rgb_t blue;
};

class Hpimage {

private:
    // All ppm files should start with magic number p6
    char magic[4];
    // Base dimensions of image loaded from memory
    uint32_t base_cols;
    uint32_t base_rows;
    // Current dimensions after shrinking
    uint32_t current_cols;
    uint32_t current_rows;
    // Maximum RGB value present in image
    uint16_t max_value;
    // Collection of all pixels in the image.
    pixel *pixels;

public:
    /**
     * Hpimage constructor.
     * This should read in all relevant fields from file.
     * @param filename Name of file to read from
     */
    Hpimage(const char *filename);

    // ACCCESSORS

    pixel get_pixel(uint32_t col, uint32_t row) const;

    size_t num_cols() const;

    size_t num_rows() const;

    // MUTATORS

    void set_pixel(uint32_t col, uint32_t row, pixel p);
    // These reduce width, height by one for SeamCarving
    // Note: these will fail if their relevant fields would be reduced to zero.

    void cut_row();

    void cut_col();

    // FS manipulators

    /**
     * Write this image class to the specified path.
     * @param filename Name of file to write out to.
     */
    void write_image(const char *filename) const;
};

} // End Hpimage namespace

#endif //HPIMAGE_HPIMAGE_H
