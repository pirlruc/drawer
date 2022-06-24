#include <gtest/gtest.h>

#include <improc_drawer_test_config.hpp>

#include <improc/drawer/drawer_types/image_file_drawer.hpp>
#include <improc/infrastructure/filesystem/file.hpp>
#include <improc/infrastructure/context/application_context.hpp>

TEST(ImageFileDrawer,TestConstructor) {
    EXPECT_NO_THROW(improc::ImageFileDrawer());
}

TEST(ImageFileDrawer,TestEmptyDraw) {
    improc::ImageFileDrawer drawer {};
    EXPECT_NO_THROW(drawer.Draw());
    EXPECT_TRUE(drawer.Verify(cv::Mat()));
}

TEST(ImageFileDrawer,TestConstructorWithLoad) {
    improc::ApplicationContext::get()->set_application_folder(std::string(IMPROC_DRAWER_TEST_FOLDER));
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/image_file_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    EXPECT_NO_THROW(improc::ImageFileDrawer {json_content});
}

TEST(ImageFileDrawer,TestLoading) {
    improc::ApplicationContext::get()->set_application_folder(std::string(IMPROC_DRAWER_TEST_FOLDER));
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/image_file_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::ImageFileDrawer drawer {};
    EXPECT_NO_THROW(drawer.Load(json_content));
}

TEST(ImageFileDrawer,TestNoImageFile) {
    improc::ApplicationContext::get()->set_application_folder(std::string(IMPROC_DRAWER_TEST_FOLDER));
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/image_file_drawer_no_image_file.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::ImageFileDrawer drawer {};
    EXPECT_THROW(drawer.Load(json_content),improc::file_processing_error);
}

TEST(ImageFileDrawer,TestInvalidImageFile) {
    improc::ApplicationContext::get()->set_application_folder(std::string(IMPROC_DRAWER_TEST_FOLDER));
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/image_file_drawer_invalid_image_file.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::ImageFileDrawer drawer {};
    EXPECT_THROW(drawer.Load(json_content),improc::invalid_filepath);
}

TEST(ImageFileDrawer,TestDraw) {
    improc::ApplicationContext::get()->set_application_folder(std::string(IMPROC_DRAWER_TEST_FOLDER));
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/image_file_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::ImageFileDrawer drawer {};
    drawer.Load(json_content);
    cv::Mat test_mat   = drawer.Draw();
    cv::Mat false_mat1 = cv::Mat::zeros(317,382,CV_8UC1);
    cv::Mat false_mat2 = cv::Mat::zeros(300,300,CV_8UC1);
    EXPECT_EQ(test_mat.rows,317);
    EXPECT_EQ(test_mat.cols,382);
    EXPECT_TRUE(drawer.Verify(test_mat));
    EXPECT_FALSE(drawer.Verify(false_mat1));
    EXPECT_FALSE(drawer.Verify(false_mat2));
}
