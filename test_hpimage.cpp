#include "gtest/gtest.h"
#include "hpimage.h"

TEST(hpimage, load) {
    auto image = hpimage::Hpimage("3x4.ppm");
    image.write_image("test.ppm");
    auto new_image = hpimage::Hpimage("test.ppm");
    ASSERT_EQ(new_image.num_cols(), image.num_cols());
    ASSERT_EQ(new_image.num_rows(), image.num_rows());
}

TEST(hpimage, modify) {
    auto image = hpimage::Hpimage("3x4.ppm");

    auto *pixel = image.get_pixels();
    pixel->red = 0;
    pixel->blue = 0;
    pixel->green = 0;
    image.cut_col();
    image.cut_row();
    image.cut_row();

    image.write_image("test2.ppm");

    auto expected = hpimage::Hpimage("test2.ppm");
    ASSERT_EQ(2, expected.num_cols());
    ASSERT_EQ(2, expected.num_rows());

    auto *new_pixels = expected.get_pixels();
    // Modified pixel should be written out as change.
    ASSERT_EQ(0, new_pixels->red);
    ASSERT_EQ(0, new_pixels->blue);
    ASSERT_EQ(0, new_pixels->green);

    // Unmodified pixels should be unaffected by width resizing.
    ASSERT_EQ(pixel[1].red, new_pixels[1].red);
    ASSERT_EQ(pixel[1].blue, new_pixels[1].blue);
    ASSERT_EQ(pixel[1].green, new_pixels[1].green);
}
