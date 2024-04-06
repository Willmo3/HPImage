#include "gtest/gtest.h"
#include "hpimage.h"

TEST(hpimage, load) {
    auto image = hpimage::Hpimage("3x4.ppm");
}
