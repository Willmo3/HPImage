#include "gtest/gtest.h"
#include "hpimage.h"

TEST(hpimage, load) {
    auto image = hpimage::Hpimage("3x4.ppm");
    image.write_image("test.ppm");
    auto new_image = hpimage::Hpimage("test.ppm");
    ASSERT_EQ(new_image.cols(), image.cols());
    ASSERT_EQ(new_image.rows(), image.rows());
}

TEST(hpimage, modify) {
    auto image = hpimage::Hpimage("3x4.ppm");

    auto pixel = image.get_pixel(0, 0);
    pixel.red = 0;
    pixel.blue = 0;
    pixel.green = 0;
    image.set_pixel(0, 0, pixel);
    image.cut_col();
    image.cut_row();
    image.cut_row();

    image.write_image("test2.ppm");

    auto expected = hpimage::Hpimage("test2.ppm");
    ASSERT_EQ(2, expected.cols());
    ASSERT_EQ(2, expected.rows());

    auto new_pixel = expected.get_pixel(0, 0);
    // Modified pixel should be written out as change.
    ASSERT_EQ(0, new_pixel.red);
    ASSERT_EQ(0, new_pixel.blue);
    ASSERT_EQ(0, new_pixel.green);

    // Unmodified pixels should be unaffected by width resizing.
    auto next_pixel = image.get_pixel(1, 0);
    auto next_new_pixel = expected.get_pixel(1, 0);
    ASSERT_EQ(next_pixel.red, next_new_pixel.red);
    ASSERT_EQ(next_pixel.blue, next_new_pixel.blue);
    ASSERT_EQ(next_pixel.green, next_new_pixel.green);
}
