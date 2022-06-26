#include <gtest/gtest.h>

#include <improc/drawer/parsers/rotation_type.hpp>

TEST(RotationType,TestEmptyConstructor) {
    improc::RotationType rotation {};
    EXPECT_EQ(rotation,improc::RotationType::Value::k0Deg);
}

TEST(RotationType,TestConstructorFromValue) {
    improc::RotationType rotation {improc::RotationType::Value::k90Deg};
    EXPECT_EQ(rotation,improc::RotationType::Value::k90Deg);
}

TEST(RotationType,TestConstructorFromLowerString) {
    improc::RotationType rotation_0deg   {"0-deg"};
    improc::RotationType rotation_90deg  {"90-deg"};
    improc::RotationType rotation_180deg {"180-deg"};
    improc::RotationType rotation_270deg {"270-deg"};
    EXPECT_EQ(rotation_0deg  ,improc::RotationType::Value::k0Deg);
    EXPECT_EQ(rotation_90deg ,improc::RotationType::Value::k90Deg);
    EXPECT_EQ(rotation_180deg,improc::RotationType::Value::k180Deg);
    EXPECT_EQ(rotation_270deg,improc::RotationType::Value::k270Deg);
}

TEST(RotationType,TestConstructorFromUpperString) {
    improc::RotationType rotation_0deg   {"0-DEG"};
    improc::RotationType rotation_90deg  {"90-DEG"};
    improc::RotationType rotation_180deg {"180-DEG"};
    improc::RotationType rotation_270deg {"270-DEG"};
    EXPECT_EQ(rotation_0deg  ,improc::RotationType::Value::k0Deg);
    EXPECT_EQ(rotation_90deg ,improc::RotationType::Value::k90Deg);
    EXPECT_EQ(rotation_180deg,improc::RotationType::Value::k180Deg);
    EXPECT_EQ(rotation_270deg,improc::RotationType::Value::k270Deg);
}

TEST(RotationType,TestInvalidRotationConstructor) {
    EXPECT_THROW(improc::RotationType rotation {"invalid"},std::out_of_range);
}

TEST(RotationType,TestConstructorFromClass) {
    EXPECT_EQ(improc::RotationType::k0Deg  ,improc::RotationType::Value::k0Deg);
    EXPECT_EQ(improc::RotationType::k90Deg ,improc::RotationType::Value::k90Deg);
    EXPECT_EQ(improc::RotationType::k180Deg,improc::RotationType::Value::k180Deg);
    EXPECT_EQ(improc::RotationType::k270Deg,improc::RotationType::Value::k270Deg);
}

TEST(RotationType,TestToString) {
    improc::RotationType rotation_0deg   {"0-deg"};
    improc::RotationType rotation_90deg  {"90-deg"};
    improc::RotationType rotation_180deg {"180-deg"};
    improc::RotationType rotation_270deg {"270-deg"};
    EXPECT_EQ(rotation_0deg.ToString()  ,"0 Degrees");
    EXPECT_EQ(rotation_90deg.ToString() ,"90 Degrees");
    EXPECT_EQ(rotation_180deg.ToString(),"180 Degrees");
    EXPECT_EQ(rotation_270deg.ToString(),"270 Degrees");
}

TEST(RotationType,TestApplyRotation) {
    improc::RotationType rotation_0deg   {"0-deg"};
    improc::RotationType rotation_90deg  {"90-deg"};
    improc::RotationType rotation_180deg {"180-deg"};
    improc::RotationType rotation_270deg {"270-deg"};
    cv::Mat original = cv::Mat::ones(10,20,CV_8UC1);
    cv::Mat rotated  = rotation_0deg.Apply(original);
    EXPECT_EQ(rotated.cols,original.cols);
    EXPECT_EQ(rotated.rows,original.rows);
    EXPECT_EQ(cv::norm(rotation_0deg.ApplyInverse(rotated),original,cv::NORM_L1),0);
    // rotated = rotation_180deg.Apply(original);
    // EXPECT_EQ(rotated.cols,original.cols);
    // EXPECT_EQ(rotated.rows,original.rows);
    // EXPECT_EQ(cv::norm(rotation_180deg.ApplyInverse(rotated),original,cv::NORM_L1),0);
    // rotated = rotation_90deg.Apply(original);
    // EXPECT_EQ(rotated.cols,original.rows);
    // EXPECT_EQ(rotated.rows,original.cols);
    // EXPECT_EQ(cv::norm(rotation_90deg.ApplyInverse(rotated),original,cv::NORM_L1),0);
    // rotated = rotation_270deg.Apply(original);
    // EXPECT_EQ(rotated.cols,original.rows);
    // EXPECT_EQ(rotated.rows,original.cols);
    // EXPECT_EQ(cv::norm(rotation_270deg.ApplyInverse(rotated),original,cv::NORM_L1),0);
}