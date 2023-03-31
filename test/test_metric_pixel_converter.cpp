#include <gtest/gtest.h>

#include <improc/drawer/engine/metric_pixel_converter.hpp>

TEST(MetricUnit,TestEmptyConstructor) {
    improc::MetricUnit metric {};
    EXPECT_EQ(metric,improc::MetricUnit::Value::kMilimiter);
}

TEST(MetricUnit,TestConstructorFromValue) {
    improc::MetricUnit metric {improc::MetricUnit::Value::kCentimeter};
    EXPECT_EQ(metric,improc::MetricUnit::Value::kCentimeter);
}

TEST(MetricUnit,TestConstructorFromLowerString) {
    improc::MetricUnit metric_m  {"m"};
    improc::MetricUnit metric_dm {"dm"};
    improc::MetricUnit metric_cm {"cm"};
    improc::MetricUnit metric_mm {"mm"};
    EXPECT_EQ(metric_m ,improc::MetricUnit::Value::kMeter);
    EXPECT_EQ(metric_dm,improc::MetricUnit::Value::kDecimeter);
    EXPECT_EQ(metric_cm,improc::MetricUnit::Value::kCentimeter);
    EXPECT_EQ(metric_mm,improc::MetricUnit::Value::kMilimiter);
}

TEST(MetricUnit,TestConstructorFromUpperString) {
    improc::MetricUnit metric_m  {"M"};
    improc::MetricUnit metric_dm {"DM"};
    improc::MetricUnit metric_cm {"CM"};
    improc::MetricUnit metric_mm {"MM"};
    EXPECT_EQ(metric_m ,improc::MetricUnit::Value::kMeter);
    EXPECT_EQ(metric_dm,improc::MetricUnit::Value::kDecimeter);
    EXPECT_EQ(metric_cm,improc::MetricUnit::Value::kCentimeter);
    EXPECT_EQ(metric_mm,improc::MetricUnit::Value::kMilimiter);
}

TEST(MetricUnit,TestInvalidmetricConstructor) {
    EXPECT_THROW(improc::MetricUnit metric {"invalid"},std::out_of_range);
}

TEST(MetricUnit,TestConstructorFromClass) {
    EXPECT_EQ(improc::MetricUnit::kMeter     ,improc::MetricUnit::Value::kMeter);
    EXPECT_EQ(improc::MetricUnit::kDecimeter ,improc::MetricUnit::Value::kDecimeter);
    EXPECT_EQ(improc::MetricUnit::kCentimeter,improc::MetricUnit::Value::kCentimeter);
    EXPECT_EQ(improc::MetricUnit::kMilimiter ,improc::MetricUnit::Value::kMilimiter);
}

TEST(MetricUnit,TestToString) {
    improc::MetricUnit metric_m  {"m"};
    improc::MetricUnit metric_dm {"dm"};
    improc::MetricUnit metric_cm {"cm"};
    improc::MetricUnit metric_mm {"mm"};
    EXPECT_EQ(metric_m.ToString() ,"Meter");
    EXPECT_EQ(metric_dm.ToString(),"Decimeter");
    EXPECT_EQ(metric_cm.ToString(),"Centimeter");
    EXPECT_EQ(metric_mm.ToString(),"Milimeter");
}

TEST(MetricUnit,TestConversionFactor) {
    improc::MetricUnit metric_cm {"cm"};
    EXPECT_DOUBLE_EQ(metric_cm.GetConversionFactor(improc::MetricUnit::kMilimiter),10.0);
    EXPECT_DOUBLE_EQ(metric_cm.GetConversionFactor(improc::MetricUnit::kCentimeter),1.0);
    EXPECT_DOUBLE_EQ(metric_cm.GetConversionFactor(improc::MetricUnit::kDecimeter),0.1);
    EXPECT_DOUBLE_EQ(metric_cm.GetConversionFactor(improc::MetricUnit::kMeter),0.01);
}

TEST(MetricPixelConverter,TestEmptyConstructor) {
    improc::MetricPixelConverter converter {};
    EXPECT_EQ(converter.Metric2Pixel(25.4),600);
}

TEST(MetricPixelConverter,TestConstructorFromValue) {
    improc::MetricPixelConverter converter {500};
    EXPECT_EQ(converter.Metric2Pixel(25.4),500);
    EXPECT_EQ(converter.Metric2Pixel(2.54,improc::MetricUnit::kCentimeter),500);
}

TEST(MetricPixelConverter,TestSetPrintingResolution) {
    improc::MetricPixelConverter converter {};
    converter.set_printing_resolution(500);
    EXPECT_EQ(converter.Metric2Pixel(25.4),500);
}

TEST(MetricPixelConverter,TestInvalidPrintingResolution) {
    improc::MetricPixelConverter converter {};
    EXPECT_THROW(converter.set_printing_resolution(0),improc::value_error);
}

TEST(MetricPixelConverter,TestConverters) {
    improc::MetricPixelConverter converter {};
    converter.set_printing_resolution(500);
    EXPECT_EQ(converter.Pixel2Metric(converter.Metric2Pixel(25.4)),25.4);
    EXPECT_EQ(converter.Pixel2Metric(converter.Metric2Pixel(-25.4)),-25.4);
    EXPECT_EQ(converter.Pixel2Metric(converter.Metric2Pixel(25.4),improc::MetricUnit::kCentimeter),2.54);
}
