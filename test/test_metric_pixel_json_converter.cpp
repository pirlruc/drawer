#include <gtest/gtest.h>

#include <improc_drawer_test_config.hpp>

#include <improc/drawer/metric_pixel_json_converter.hpp>
#include <improc/infrastructure/filesystem/file.hpp>

TEST(MetricPixelJsonConverter,TestEmptyConstructor) {
    EXPECT_NO_THROW(improc::MetricPixelJsonConverter());
}

TEST(MetricPixelJsonConverter,TestEmptyConversion) {
    improc::MetricPixelJsonConverter converter {};
    Json::Value json_empty {};
    EXPECT_THROW(converter.Convert(json_empty),improc::file_processing_error);
}

TEST(MetricPixelJsonConverter,TestNoPrintingResolution) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/metric_no_printing_resolution.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::MetricPixelJsonConverter converter {};
    EXPECT_THROW(converter.Convert(json_content),improc::file_processing_error);
}

TEST(MetricPixelJsonConverter,TestNoLayout) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/metric_no_layout.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::MetricPixelJsonConverter converter {};
    EXPECT_THROW(converter.Convert(json_content),improc::file_processing_error);
}

TEST(MetricPixelJsonConverter,TestNoMetricUnit) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/metric_no_metric_unit.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::MetricPixelJsonConverter converter {};
    EXPECT_THROW(converter.Convert(json_content),improc::file_processing_error);
}

TEST(MetricPixelJsonConverter,TestConversion) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/metric_layout_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::MetricPixelJsonConverter converter {};
    EXPECT_NO_THROW(converter.Convert(json_content));
}

