#include <gtest/gtest.h>

#include <improc_drawer_test_config.hpp>

#include <improc/drawer/drawer_types/barcode_drawer.hpp>
#include <improc/infrastructure/filesystem/file.hpp>

TEST(BarcodeDrawer,TestConstructor) {
    EXPECT_NO_THROW(improc::BarcodeDrawer());
}

TEST(BarcodeDrawer,TestEmptyDraw) {
    improc::BarcodeDrawer drawer {};
    EXPECT_THROW(drawer.Draw(),std::bad_optional_access);
    EXPECT_NO_THROW(drawer.Draw("test_message"));
}

TEST(BarcodeDrawer,TestConstructorWithLoad) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/barcode_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    EXPECT_NO_THROW(improc::BarcodeDrawer {json_content});
}

TEST(BarcodeDrawer,TestLoading) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/barcode_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::BarcodeDrawer drawer {};
    EXPECT_NO_THROW(drawer.Load(json_content));
}

TEST(BarcodeDrawer,TestDraw) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/barcode_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::BarcodeDrawer drawer {};
    drawer.Load(json_content);
    cv::Mat test_mat = drawer.Draw("test_message");
    EXPECT_EQ(test_mat.rows,10);
    EXPECT_EQ(test_mat.cols,167);
}
