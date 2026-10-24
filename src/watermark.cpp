#include "watermark.h"
#include "imageBMP.h"

ImageBMP ApplyWatermark(const ImageBMP& original, const ImageBMP& watermark) {
    if (watermark.width() > original.width() || watermark.height() > original.height()) {
        std::cerr << "Error: the watermark must not be larger than the original image!" << std::endl;
        return;
    }//verification of dimensional acceptability
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
