find_package(GTest REQUIRED)

add_executable(bmp_tests
    applyWatermark_test/applyWatermark_test.cpp
    color_test/color_test.cpp
    imagebmp_test/imagebmp_test.cpp
    bmphandler_test/bmphandler_test.cpp
)

target_compile_definitions(bmp_tests PRIVATE
    TEST_CASES_DIR="${CMAKE_SOURCE_DIR}/tests/bmphandler_test/test_cases"
)

target_link_libraries(bmp_tests PRIVATE
    bmp_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(bmp_tests)
