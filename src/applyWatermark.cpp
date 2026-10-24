#include <bits/stdc++.h>
#include "imageBMP.h"
#include "color.h"

ImageBMP ApplyWatermark(const ImageBMP& original, const ImageBMP& watermark) {
    if (watermark.width() > original.width() || watermark.height() > original.height()) {
        std::cout << "водяной знак не должен быть больше исходного изображения";
        return;
    }//проверка что размеры подходят
    ImageBMP result = original;
    for (std::size_t y = 0; y < watermark.GetHeight(); ++y) {
        for (std::size_t x = 0; x < watermark.GetWidth(); ++x) {
            const Color& a = original.Pixel(y, x);
            const Color& b = watermark.Pixel(y, x);
            result.Pixel(y, x) = Color((uint8_t)((7 * a.R + 3 * b.R) / 10), (uint8_t)((7 * a.G + 3 * b.G) / 10), (uint8_t)((7 * a.B + 3 * b.B) / 10) );
        }
    }
    return result;
}
