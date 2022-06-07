#include <gtest/gtest.h>

#include <improc_drawer_test_config.hpp>

#include <improc/drawer/drawer_types/qrcode_drawer.hpp>
#include <improc/infrastructure/filesystem/file.hpp>

TEST(ErrorCorrectionLevel,TestEmptyConstructor) {
    improc::qrcode::ErrorCorrectionLevel ecc {};
    EXPECT_EQ(ecc,improc::qrcode::ErrorCorrectionLevel::Value::kLow);
}

TEST(ErrorCorrectionLevel,TestConstructorFromValue) {
    improc::qrcode::ErrorCorrectionLevel ecc {improc::qrcode::ErrorCorrectionLevel::Value::kHigh};
    EXPECT_EQ(ecc,improc::qrcode::ErrorCorrectionLevel::Value::kHigh);
}

TEST(ErrorCorrectionLevel,TestConstructorFromLowerString) {
    improc::qrcode::ErrorCorrectionLevel ecc_low      {"low"};
    improc::qrcode::ErrorCorrectionLevel ecc_medium   {"medium"};
    improc::qrcode::ErrorCorrectionLevel ecc_quartile {"quartile"};
    improc::qrcode::ErrorCorrectionLevel ecc_high     {"high"};
    EXPECT_EQ(ecc_low     ,improc::qrcode::ErrorCorrectionLevel::Value::kLow);
    EXPECT_EQ(ecc_medium  ,improc::qrcode::ErrorCorrectionLevel::Value::kMedium);
    EXPECT_EQ(ecc_quartile,improc::qrcode::ErrorCorrectionLevel::Value::kQuartile);
    EXPECT_EQ(ecc_high    ,improc::qrcode::ErrorCorrectionLevel::Value::kHigh);
}

TEST(ErrorCorrectionLevel,TestConstructorFromUpperString) {
    improc::qrcode::ErrorCorrectionLevel ecc_low      {"LOW"};
    improc::qrcode::ErrorCorrectionLevel ecc_medium   {"MEDIUM"};
    improc::qrcode::ErrorCorrectionLevel ecc_quartile {"QUARTILE"};
    improc::qrcode::ErrorCorrectionLevel ecc_high     {"HIGH"};
    EXPECT_EQ(ecc_low     ,improc::qrcode::ErrorCorrectionLevel::Value::kLow);
    EXPECT_EQ(ecc_medium  ,improc::qrcode::ErrorCorrectionLevel::Value::kMedium);
    EXPECT_EQ(ecc_quartile,improc::qrcode::ErrorCorrectionLevel::Value::kQuartile);
    EXPECT_EQ(ecc_high    ,improc::qrcode::ErrorCorrectionLevel::Value::kHigh);
}

TEST(ErrorCorrectionLevel,TestInvalidErrorCorrectionLevelConstructor) {
    EXPECT_THROW(improc::qrcode::ErrorCorrectionLevel ecc {"invalid"},std::out_of_range);
}

TEST(ErrorCorrectionLevel,TestConstructorFromClass) {
    EXPECT_EQ(improc::qrcode::ErrorCorrectionLevel::kLow     ,improc::qrcode::ErrorCorrectionLevel::Value::kLow);
    EXPECT_EQ(improc::qrcode::ErrorCorrectionLevel::kMedium  ,improc::qrcode::ErrorCorrectionLevel::Value::kMedium);
    EXPECT_EQ(improc::qrcode::ErrorCorrectionLevel::kQuartile,improc::qrcode::ErrorCorrectionLevel::Value::kQuartile);
    EXPECT_EQ(improc::qrcode::ErrorCorrectionLevel::kHigh    ,improc::qrcode::ErrorCorrectionLevel::Value::kHigh);
}

TEST(ErrorCorrectionLevel,TestToString) {
    improc::qrcode::ErrorCorrectionLevel ecc_low      {"low"};
    improc::qrcode::ErrorCorrectionLevel ecc_medium   {"medium"};
    improc::qrcode::ErrorCorrectionLevel ecc_quartile {"quartile"};
    improc::qrcode::ErrorCorrectionLevel ecc_high     {"high"};
    EXPECT_EQ(ecc_low.ToString()     ,"Low     : The QR Code can tolerate about  7% erroneous codewords");
    EXPECT_EQ(ecc_medium.ToString()  ,"Medium  : The QR Code can tolerate about 15% erroneous codewords");
    EXPECT_EQ(ecc_quartile.ToString(),"Quartile: The QR Code can tolerate about 25% erroneous codewords");
    EXPECT_EQ(ecc_high.ToString()    ,"High    : The QR Code can tolerate about 30% erroneous codewords");
}

TEST(ErrorCorrectionLevel,TestToQrCodeGen) {
    improc::qrcode::ErrorCorrectionLevel ecc_low      {"low"};
    improc::qrcode::ErrorCorrectionLevel ecc_medium   {"medium"};
    improc::qrcode::ErrorCorrectionLevel ecc_quartile {"quartile"};
    improc::qrcode::ErrorCorrectionLevel ecc_high     {"high"};
    EXPECT_EQ(ecc_low.ToQrCodeGen()     ,qrcodegen::QrCode::Ecc::LOW);
    EXPECT_EQ(ecc_medium.ToQrCodeGen()  ,qrcodegen::QrCode::Ecc::MEDIUM);
    EXPECT_EQ(ecc_quartile.ToQrCodeGen(),qrcodegen::QrCode::Ecc::QUARTILE);
    EXPECT_EQ(ecc_high.ToQrCodeGen()    ,qrcodegen::QrCode::Ecc::HIGH);
}

TEST(QrCodeDrawer,TestConstructor) {
    EXPECT_NO_THROW(improc::QrCodeDrawer());
}

TEST(QrCodeDrawer,TestEmptyDraw) {
    improc::QrCodeDrawer drawer {};
    EXPECT_THROW(drawer.Draw(),std::bad_optional_access);
    EXPECT_NO_THROW(drawer.Draw("test_message"));
}

TEST(QrCodeDrawer,TestConstructorWithLoad) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/qrcode_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    EXPECT_NO_THROW(improc::QrCodeDrawer {json_content});
}

TEST(QrCodeDrawer,TestLoading) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/qrcode_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::QrCodeDrawer drawer {};
    EXPECT_NO_THROW(drawer.Load(json_content));
}

TEST(QrCodeDrawer,TestNoErrorCorrectionLevel) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/qrcode_drawer_no_error_correction_level.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::QrCodeDrawer drawer {};
    EXPECT_THROW(drawer.Load(json_content),improc::file_processing_error);
}

TEST(QrCodeDrawer,TestDraw) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/qrcode_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::QrCodeDrawer drawer {};
    drawer.Load(json_content);
    cv::Mat test_mat = drawer.Draw("test_message");
    EXPECT_EQ(test_mat.rows,21);
    EXPECT_EQ(test_mat.cols,21);
}
