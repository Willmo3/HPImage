#include <cassert>
#include <iostream>
#include "hpimage.h"

namespace hpimage {

// Hpimage constructor. Reads from file.
Hpimage::Hpimage(const char *filename) {
    FILE *fin = fopen(filename, "r");
    if (!fin) {
        std::cerr << "ERROR: could not open " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    // Reads the file, assuming file structure
    if (fscanf(fin, "%s\n", magic) != 1) {
        std::cerr << "ERROR: reading magic number" << std::endl;
        exit(EXIT_FAILURE);
    }

    /* ImageMagick may produce an extra line when converting the image to a PPM.
     * We first check to see if we find the two integer pattern. If it is not found, then we
     * assume the line exists and ignore it. */

    // Height == num rows in image.
    if (fscanf(fin, "%d %d\n", &base_cols, &base_rows) != 2) {
        if (fscanf(fin, "%*s\n%d %d\n", &base_cols, &base_rows) != 2) {
            std::cerr << "ERROR: reading width/height" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    if (base_cols < 1 || base_rows < 1) {
        std::cerr << "ERROR: Malformed image, width too small" << std::endl;
        exit(EXIT_FAILURE);
    }

    current_cols = base_cols;
    current_rows = base_rows;

    if (fscanf(fin, "%hd\n", &max_value) != 1) {
        std::cerr << "ERROR: reading maximum value" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Allocate room for the pixels in the image.
    uint64_t num_pixels = base_cols * base_rows;
    pixels = static_cast<pixel *>(calloc(sizeof(pixel), num_pixels));

    // Local temporary variables
    auto pix_count = 0;
    rgb_t r, g, b;

    // Begin reading pixel RGB values
    while (fscanf(fin, "%hu %hu %hu", &r, &g, &b) == 3) {
        pixel current_pix = {.red = r, .green = g, .blue = b};
        pixels[pix_count] = current_pix;
        pix_count++;
    }

    fclose(fin);
}

// ACCESSORS

size_t Hpimage::num_cols() const {
    return current_cols;
}

size_t Hpimage::num_rows() const {
    return current_rows;
}

// MUTATORS

void Hpimage::cut_col() {
    // Program invariant: height must be greater than one if cut.
    assert(current_cols > 1);
    current_cols -= 1;
}

void Hpimage::cut_row() {
    // Program invariant: width must be greater than one if cut.
    assert(current_rows > 1);
    current_rows -= 1;
}

/**
 * Writes out a PPM structure to disk.
 */
void Hpimage::write_image(const char *filename) const {
    FILE *fout = fopen(filename, "w");
    if (!fout) {
        std::cerr << "ERROR: could not open " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    if (fprintf(fout, "%s\n", magic) < 0) {
        std::cerr << "ERROR: writing magic number" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (fprintf(fout, "%d %d\n", current_cols, current_rows) < 0) {
        std::cerr << "ERROR: writing width/height" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (fprintf(fout, "%hd\n", max_value) < 0) {
        std::cerr << "ERROR: writing maximum value" << std::endl;
        exit(EXIT_FAILURE);
    }

    // the current/base width dichotomy allows the image to logically shrink
    // without reallocating the base pixel buffer.
    // We simply "skip" pixels past current width/height
    for (auto i = 0; i < current_rows; ++i) {
        for (auto j = 0; j < current_cols; ++j) {
            // But for each row, we skip through base_width pixels.
            // Subtract one for zero index
            pixel current = pixels[i * (base_rows -1) + j];
            if (fprintf(fout, "%hu %hu %hu ",
                        current.red, current.green, current.blue) < 0) {
                std::cerr << "ERROR: writing pixel value" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        // To comply with ImageMagick strategy, print newline after each row.
        if (fprintf(fout, "\n") < 0) {
            std::cerr << "ERROR: writing newline" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    fclose(fout);
}


}
