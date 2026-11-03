
#include <gtest/gtest.h>
 
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <vector>
 
#include "bmpHandler.h"
#include "imageBMP.h"
 
namespace fs = std::filesystem;
 
namespace {
 
const fs::path kTestCases = WATERMARK_CASES_DIR;
const fs::path kExecutable = WATERMARK_EXE_PATH;
 
constexpr double kOriginalWeight = 0.7;
constexpr double kWatermarkWeight = 0.3;
 
struct TestCase {
    std::string name;
    std::string original;
    std::string watermark;
};
 
const std::vector<TestCase> kCases = {
    {"test1", "test1_16x16.bmp",
     "test1_watermark.bmp"},
 
    {"test2", "test2_16x16.bmp",
     "test2_watermark.bmp"},
 
    {"test3", "test3_4x4.bmp",
     "test3_watermark.bmp"},
 
    {"test4", "test4_14x7.bmp",
     "test4_watermark.bmp"}
};
 
fs::path CasePath(const std::string& name,
                  const std::string& file) {
    return kTestCases / name / file;
}
 
fs::path OutputPath(const std::string& name) {
    return fs::current_path() /
           ("watermark_result_" + name + ".bmp");
}
 
struct RemoveOutput {
    fs::path path;
 
    ~RemoveOutput() {
        std::error_code error;
        fs::remove(path, error);
    }
};
 
bool RunWatermark(const fs::path& original,
                  const fs::path& watermark,
                  const fs::path& output) {
    std::string command =
        "\"" + kExecutable.string() + "\" \"" +
        original.string() + "\" \"" +
        watermark.string() + "\" \"" +
        output.string() + "\"";
 
    return std::system(command.c_str()) == 0;
}
 
void ExpectColorNear(const Color& original,
                     const Color& watermark,
                     const Color& actual) {
    const auto expected = [](int a, int b) {
        return a * kOriginalWeight +
               b * kWatermarkWeight;
    };
 
    EXPECT_NEAR(
        static_cast<int>(actual.R),
        expected(original.R, watermark.R), 1.0);
 
    EXPECT_NEAR(
        static_cast<int>(actual.G),
        expected(original.G, watermark.G), 1.0);
 
    EXPECT_NEAR(
        static_cast<int>(actual.B),
        expected(original.B, watermark.B), 1.0);
}
 
void CheckWatermark(const TestCase& test) {
    SCOPED_TRACE(test.name);
 
    const fs::path original_path =
        CasePath(test.name, test.original);
 
    const fs::path watermark_path =
        CasePath(test.name, test.watermark);
 
    const fs::path output_path =
        OutputPath(test.name);
 
    RemoveOutput cleanup{output_path};
 
    ASSERT_TRUE(fs::exists(original_path));
    ASSERT_TRUE(fs::exists(watermark_path));
 
    ImageBMP original;
    ImageBMP watermark;
    ImageBMP result;
 
    ASSERT_TRUE(HandlerBMP::Read(
        original_path.string().c_str(), original));
 
    ASSERT_TRUE(HandlerBMP::Read(
        watermark_path.string().c_str(), watermark));
 
    ASSERT_LE(watermark.GetWidth(),
              original.GetWidth());
 
    ASSERT_LE(watermark.GetHeight(),
              original.GetHeight());
 
    ASSERT_TRUE(RunWatermark(
        original_path, watermark_path, output_path));
 
    ASSERT_TRUE(fs::exists(output_path));
 
    ASSERT_TRUE(HandlerBMP::Read(
        output_path.string().c_str(), result));
 
    ASSERT_EQ(result.GetWidth(), original.GetWidth());
    ASSERT_EQ(result.GetHeight(), original.GetHeight());
 
    for (size_t r = 0; r < original.GetHeight(); ++r) {
        for (size_t c = 0; c < original.GetWidth(); ++c) {
            SCOPED_TRACE(
                "row=" + std::to_string(r) +
                ", col=" + std::to_string(c));
 
            if (r < watermark.GetHeight() &&
                c < watermark.GetWidth()) {
 
                ExpectColorNear(
                    original.Pixel(r, c),
                    watermark.Pixel(r, c),
                    result.Pixel(r, c));
 
            } else {
                EXPECT_EQ(
                    result.Pixel(r, c),
                    original.Pixel(r, c));
            }
        }
    }
}
 
} // namespace
 
TEST(ApplyWatermarkTest, Test1) {
    CheckWatermark(kCases[0]);
}
 
TEST(ApplyWatermarkTest, Test2) {
    CheckWatermark(kCases[1]);
}
 
TEST(ApplyWatermarkTest, Test3) {
    CheckWatermark(kCases[2]);
}
 
TEST(ApplyWatermarkTest, Test4) {
    CheckWatermark(kCases[3]);
}
 
TEST(ApplyWatermarkTest, InvalidWatermarkSize) {
    const fs::path original =
        CasePath("test2", "test2_16x16.bmp");
 
    const fs::path watermark =
        CasePath("test2", "test2_watermark_error.bmp");
 
    const fs::path output =
        OutputPath("test2_error");
 
    RemoveOutput cleanup{output};
 
    ImageBMP source;
    ImageBMP invalid;
 
    ASSERT_TRUE(HandlerBMP::Read(
        original.string().c_str(), source));
 
    ASSERT_TRUE(HandlerBMP::Read(
        watermark.string().c_str(), invalid));
 
    ASSERT_TRUE(
        invalid.GetWidth() > source.GetWidth() ||
        invalid.GetHeight() > source.GetHeight());
 
    EXPECT_FALSE(RunWatermark(
        original, watermark, output));
}
 