#include <gtest/gtest.h>

#include <improc_drawer_test_config.hpp>

#include <improc/drawer/drawer_types/data_matrix_drawer.hpp>
#include <improc/infrastructure/filesystem/file.hpp>

TEST(DataMatrixDrawer,TestConstructor) {
    EXPECT_NO_THROW(improc::DataMatrixDrawer());
}

TEST(DataMatrixDrawer,TestEmptyDraw) {
    improc::DataMatrixDrawer drawer {};
    EXPECT_THROW(drawer.Draw(),std::bad_optional_access);
    EXPECT_NO_THROW(drawer.Draw("test_message"));
    EXPECT_FALSE(drawer.Verify(cv::Mat()));
    EXPECT_FALSE(drawer.Verify(cv::Mat(), "test_message"));
}

TEST(DataMatrixDrawer,TestConstructorWithLoad) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/data_matrix_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    EXPECT_NO_THROW(improc::DataMatrixDrawer {json_content});
}

TEST(DataMatrixDrawer,TestLoading) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/data_matrix_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DataMatrixDrawer drawer {};
    EXPECT_NO_THROW(drawer.Load(json_content));
}

TEST(DataMatrixDrawer,TestDraw) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/data_matrix_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DataMatrixDrawer drawer {};
    drawer.Load(json_content);
    cv::Mat test_mat  = drawer.Draw("test_message");
    cv::Mat false_mat = cv::Mat::zeros(test_mat.rows,test_mat.cols,test_mat.type());
    EXPECT_EQ(test_mat.rows,16);
    EXPECT_EQ(test_mat.cols,16);
    EXPECT_TRUE(drawer.Verify(test_mat,"test_message"));
    EXPECT_FALSE(drawer.Verify(false_mat,"test_message"));
}
