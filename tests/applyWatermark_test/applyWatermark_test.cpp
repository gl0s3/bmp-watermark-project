
#include <gtest/gtest.h>

#include "watermark.h"
#include "imageBMP.h"
#include "color.h"

TEST(ApplyWatermarkTest, BlendColors) {
    ImageBMP original(1, 1);
    ImageBMP watermark(1, 1);

    original.Pixel(0, 0) = Color(100, 150, 200);
    watermark.Pixel(0, 0) = Color(200, 50, 100);

    ImageBMP result = ApplyWatermark(original, watermark);

    EXPECT_EQ(result.Pixel(0, 0),
              Color(130, 120, 170));
}

TEST(ApplyWatermarkTest, MaximumColors) {
    ImageBMP original(1, 1);
    ImageBMP watermark(1, 1);

    original.Pixel(0, 0) = Color(255, 255, 255);
    watermark.Pixel(0, 0) = Color(255, 255, 255);

    ImageBMP result = ApplyWatermark(original, watermark);

    EXPECT_EQ(result.Pixel(0, 0),
              Color(255, 255, 255));
}

TEST(ApplyWatermarkTest, SmallerWatermark) {
    ImageBMP original(3, 3);
    ImageBMP watermark(1, 1);

    for (size_t y = 0; y < 3; ++y) {
        for (size_t x = 0; x < 3; ++x) {
            original.Pixel(y, x) = Color(100, 100, 100);
        }
    }

    watermark.Pixel(0, 0) = Color(200, 200, 200);

    ImageBMP result = ApplyWatermark(original, watermark);

    EXPECT_EQ(result.GetWidth(), 3);
    EXPECT_EQ(result.GetHeight(), 3);

    EXPECT_EQ(result.Pixel(0, 0), Color(130, 130, 130));

    for (size_t y = 0; y < 3; ++y) {
        for (size_t x = 0; x < 3; ++x) {
            if (y == 0 && x == 0) {
                continue;
            }

            EXPECT_EQ(result.Pixel(y, x),
                      Color(100, 100, 100));
        }
    }
}

TEST(ApplyWatermarkTest, InvalidSize) {
    ImageBMP original(2, 2);
    ImageBMP watermark(3, 3);

    ImageBMP result = ApplyWatermark(original, watermark);

    EXPECT_EQ(result.GetWidth(), 0);
    EXPECT_EQ(result.GetHeight(), 0);
}