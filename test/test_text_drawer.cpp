#include <gtest/gtest.h>

#include <improc_drawer_test_config.hpp>

#include <improc/drawer/drawer_types/text_drawer.hpp>
#include <improc/infrastructure/filesystem/file.hpp>

TEST(TextRenderMode,TestEmptyConstructor) {
    improc::TextRenderMode render {};
    EXPECT_EQ(render,improc::TextRenderMode::Value::kGrayscale);
}

TEST(TextRenderMode,TestConstructorFromValue) {
    improc::TextRenderMode render {improc::TextRenderMode::Value::kBinary};
    EXPECT_EQ(render,improc::TextRenderMode::Value::kBinary);
}

TEST(TextRenderMode,TestConstructorFromLowerString) {
    improc::TextRenderMode render_binary      {"binary"};
    improc::TextRenderMode render_grayscale   {"grayscale"};
    EXPECT_EQ(render_binary   ,improc::TextRenderMode::Value::kBinary);
    EXPECT_EQ(render_grayscale,improc::TextRenderMode::Value::kGrayscale);
}

TEST(TextRenderMode,TestConstructorFromUpperString) {
    improc::TextRenderMode render_binary      {"BINARY"};
    improc::TextRenderMode render_grayscale   {"GRAYSCALE"};
    EXPECT_EQ(render_binary   ,improc::TextRenderMode::Value::kBinary);
    EXPECT_EQ(render_grayscale,improc::TextRenderMode::Value::kGrayscale);
}

TEST(TextRenderMode,TestInvalidTextRenderModeConstructor) {
    EXPECT_THROW(improc::TextRenderMode render {"invalid"},std::out_of_range);
}

TEST(TextRenderMode,TestConstructorFromClass) {
    EXPECT_EQ(improc::TextRenderMode::kBinary   ,improc::TextRenderMode::Value::kBinary);
    EXPECT_EQ(improc::TextRenderMode::kGrayscale,improc::TextRenderMode::Value::kGrayscale);
}

TEST(TextRenderMode,TestToString) {
    improc::TextRenderMode render_binary      {"binary"};
    improc::TextRenderMode render_grayscale   {"grayscale"};
    EXPECT_EQ(render_binary.ToString()     ,"Binary rendering");
    EXPECT_EQ(render_grayscale.ToString()  ,"Grayscale rendering");
}

TEST(TextDrawer,TestConstructor) {
    EXPECT_NO_THROW(improc::TextDrawer());
}

TEST(TextDrawer,TestEmptyDraw) {
    improc::TextDrawer drawer {};
    EXPECT_THROW(drawer.Draw(),improc::freetype_lib_error);
    EXPECT_THROW(drawer.Draw("test_message"),improc::freetype_lib_error);
}

TEST(TextDrawer,TestConstructorWithLoad) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/text_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    EXPECT_NO_THROW(improc::TextDrawer {json_content});
}

TEST(TextDrawer,TestLoading) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/text_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::TextDrawer drawer {};
    EXPECT_NO_THROW(drawer.Load(json_content));
}

TEST(TextDrawer,TestNoPrintingResolution) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/text_drawer_no_printing_resolution.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::TextDrawer drawer {};
    EXPECT_THROW(drawer.Load(json_content),improc::file_processing_error);
}

TEST(TextDrawer,TestNoImageTextSize) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/text_drawer_no_image_text_size.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::TextDrawer drawer {};
    EXPECT_THROW(drawer.Load(json_content),improc::file_processing_error);
}

TEST(TextDrawer,TestNoFontPath) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/text_drawer_no_font_filepath.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::TextDrawer drawer {};
    EXPECT_THROW(drawer.Load(json_content),improc::file_processing_error);
}

TEST(TextDrawer,TestNoFontSize) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/text_drawer_no_font_size.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::TextDrawer drawer {};
    EXPECT_THROW(drawer.Load(json_content),improc::file_processing_error);
}

TEST(TextDrawer,TestNoFontSpacing) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/text_drawer_no_font_spacing.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::TextDrawer drawer {};
    EXPECT_THROW(drawer.Load(json_content),improc::file_processing_error);
}

TEST(TextDrawer,TestNoRenderMode) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/text_drawer_no_render_mode.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::TextDrawer drawer {};
    EXPECT_THROW(drawer.Load(json_content),improc::file_processing_error);
}

TEST(TextDrawer,TestNoMetricUnit) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/text_drawer_no_metric_unit.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::TextDrawer drawer {};
    EXPECT_THROW(drawer.Load(json_content),improc::file_processing_error);
}

TEST(TextDrawer,TestInvalidWidth) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/text_drawer_invalid_width.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::TextDrawer drawer {};
    EXPECT_THROW(drawer.Load(json_content),improc::file_processing_error);
}

TEST(TextDrawer,TestInvalidHeight) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/text_drawer_invalid_height.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::TextDrawer drawer {};
    EXPECT_THROW(drawer.Load(json_content),improc::file_processing_error);
}

TEST(TextDrawer,TestInvalidFontFilepath) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/text_drawer_invalid_font_filepath.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::TextDrawer drawer {};
    EXPECT_THROW(drawer.Load(json_content),improc::invalid_filepath);
}

TEST(TextDrawer,TestInvalidFontSize) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/text_drawer_invalid_font_size.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::TextDrawer drawer {};
    EXPECT_THROW(drawer.Load(json_content),improc::file_processing_error);
}

TEST(TextDrawer,TestInvalidFontSpacing) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/text_drawer_invalid_font_spacing.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::TextDrawer drawer {};
    EXPECT_THROW(drawer.Load(json_content),improc::file_processing_error);
}

TEST(TextDrawer,TestDrawOutsideImage) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/text_drawer_outside_image.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::TextDrawer drawer {};
    drawer.Load(json_content);
    EXPECT_THROW(drawer.Draw("test"),cv::Exception);
}

TEST(TextDrawer,TestDrawGrayscale) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/text_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::TextDrawer drawer {};
    drawer.Load(json_content);
    cv::Mat test_mat = drawer.Draw("test");
    EXPECT_EQ(test_mat.rows,118);
    EXPECT_EQ(test_mat.cols,197);
}
