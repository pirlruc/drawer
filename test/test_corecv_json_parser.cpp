#include <gtest/gtest.h>

#include <improc_drawer_test_config.hpp>

#include <improc/drawer/parsers/corecv_json_parser.hpp>
#include <improc/infrastructure/filesystem/file.hpp>

TEST(PointParser,TestNoTopLeftX) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_element_drawer_no_top_left_x.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    EXPECT_THROW(improc::json::ReadElement<cv::Point>(json_content["top-left"]),improc::file_processing_error);
}

TEST(PointParser,TestNoTopLeftY) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_element_drawer_no_top_left_y.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    EXPECT_THROW(improc::json::ReadElement<cv::Point>(json_content["top-left"]),improc::file_processing_error);
}

TEST(SizeParser,TestNoPageWidth) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_drawer_no_page_width.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    EXPECT_THROW(improc::json::ReadElement<cv::Size>(json_content["page-size"]),improc::file_processing_error);
}

TEST(SizeParser,TestNoPageHeight) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_drawer_no_page_height.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    EXPECT_THROW(improc::json::ReadElement<cv::Size>(json_content["page-size"]),improc::file_processing_error);
}