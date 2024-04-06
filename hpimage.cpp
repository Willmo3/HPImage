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
    if (fscanf(fin, "%d %d\n", &base_width, &base_height) != 2) {
        if (fscanf(fin, "%*s\n%d %d\n", &base_width, &base_height) != 2) {
            std::cerr << "ERROR: reading width/height" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    if (base_width < 1 || base_height < 1) {
        std::cerr << "ERROR: Malformed image, width too small" << std::endl;
        exit(EXIT_FAILURE);
    }

    current_width = base_width;
    current_height = base_height;

    if (fscanf(fin, "%hd\n", &max_value) != 1) {
        std::cerr << "ERROR: reading maximum value" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Allocate room for the pixels in the image.
    uint64_t num_pixels = base_width * base_height;
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

size_t Hpimage::get_width() const {
    return current_width;
}

size_t Hpimage::get_height() const {
    return current_height;
}

// MUTATORS

void Hpimage::cut_height() {
    // Program invariant: height must be greater than one if cut.
    assert(current_height > 1);
    current_height -= 1;
}

void Hpimage::cut_width() {
    // Program invariant: width must be greater than one if cut.
    assert(current_width > 1);
    current_width -= 1;
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
    if (fprintf(fout, "%d %d\n", current_width, current_height) < 0) {
        std::cerr << "ERROR: writing width/height" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (fprintf(fout, "%hd\n", max_value) < 0) {
        std::cerr << "ERROR: writing maximum value" << std::endl;
        exit(EXIT_FAILURE);
    }

    auto pix_count = 0;

    // the current/base width dichotomy allows the image to logically shrink
    // without reallocating the base pixel buffer.
    // We simply "skip" pixels past current width/height
    for (auto i = 0; i < current_width; ++i) {
        for (auto j = 0; j < current_height; ++j) {
            // But for each row, we skip through base_width pixels.
            pixel current = pixels[i * base_width + j];

            // Add newlines for every 60 pixels written.
            if (pix_count % MAX_PIXELS_PER_LINE == 0) {
                if (fprintf(fout, "\n") < 0) {
                    std::cerr << "ERROR: writing maximum value" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }

            if (fprintf(fout, "%hu %hu %hu ",
                        current.red, current.green, current.blue) < 0) {
                std::cerr << "ERROR: writing pixel value" << std::endl;
                exit(EXIT_FAILURE);
            }

            ++pix_count;
        }
    }

    fclose(fout);
}


}
