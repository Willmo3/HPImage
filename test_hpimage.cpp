#include "gtest/gtest.h"
#include "hpimage.h"

TEST(hpimage, load) {
    auto image = hpimage::Hpimage("3x4.ppm");
    image.write_image("test.ppm");
    auto new_image = hpimage::Hpimage("test.ppm");
    ASSERT_EQ(new_image.num_cols(), image.num_cols());
    ASSERT_EQ(new_image.num_rows(), image.num_rows());
}
