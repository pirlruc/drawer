#include <gtest/gtest.h>

#include <improc_drawer_test_config.hpp>

#include <improc/drawer/parsers/corecv_json_parser.hpp>
#include <improc/infrastructure/filesystem/file.hpp>

TEST(PointParser,TestParsing) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/point_parser.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    EXPECT_EQ(improc::json::ReadElement<cv::Point>(json_content["point-int"]),cv::Point(6,10));
    EXPECT_EQ(improc::json::ReadElement<cv::Point2d>(json_content["point-double"]),cv::Point2d(6.3,10.6));
}

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

TEST(SizeParser,TestParsing) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/size_parser.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    EXPECT_EQ(improc::json::ReadElement<cv::Size>(json_content["size-int"]),cv::Size(6,10));
    EXPECT_EQ(improc::json::ReadElement<cv::Size2d>(json_content["size-double"]),cv::Size2d(6.3,10.6));
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

TEST(PositiveSizeParser,TestInvalidWidth) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_drawer_invalid_page_width.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    EXPECT_THROW(improc::json::ReadPositiveSize<cv::Size>(json_content["page-size"]),improc::file_processing_error);
    EXPECT_THROW(improc::json::ReadPositiveSize<cv::Size2d>(json_content["page-size"]),improc::file_processing_error);
}

TEST(PositiveSizeParser,TestInvalidHeight) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_drawer_invalid_page_height.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    EXPECT_THROW(improc::json::ReadPositiveSize<cv::Size>(json_content["page-size"]),improc::file_processing_error);
    EXPECT_THROW(improc::json::ReadPositiveSize<cv::Size2d>(json_content["page-size"]),improc::file_processing_error);
}
