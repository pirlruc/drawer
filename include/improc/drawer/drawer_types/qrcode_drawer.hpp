#ifndef IMPROC_DRAWER_QRCODE_DRAWER_HPP
#define IMPROC_DRAWER_QRCODE_DRAWER_HPP

#include <improc/improc_defs.hpp>
#include <improc/infrastructure/string.hpp>
#include <improc/drawer/engine/base_drawer.hpp>

#include <qrcodegen/cpp/qrcodegen.hpp>
#include <BitMatrix.h>
#include <BinaryBitmap.h>
#include <DecodeHints.h>
#include <DecoderResult.h>
#include <ThresholdBinarizer.h>
#include <qrcode/QRDecoder.h>
#include <codecvt>
#include <locale>

namespace improc 
{
    namespace qrcode
    {
        class IMPROC_EXPORTS ErrorCorrectionLevel
        {
            public:
                enum Value : IMPROC_ENUM_KEY_TYPE
                {
                        kLow      = 0
                    ,   kMedium   = 1
                    ,   kQuartile = 2
                    ,   kHigh     = 3
                };

            private:
                Value                               value_;

            public:
                ErrorCorrectionLevel();                              
                ErrorCorrectionLevel(const std::string& error_correction_level_str);
                constexpr                           ErrorCorrectionLevel(Value error_correction_level_value): value_(error_correction_level_value) {}
                constexpr operator                  Value()     const {return this->value_;}

                constexpr std::string_view          ToString()  const
                {
                    switch (this->value_)
                    {
                        case ErrorCorrectionLevel::Value::kLow     : return "Low     : The QR Code can tolerate about  7% erroneous codewords";  break;
                        case ErrorCorrectionLevel::Value::kMedium  : return "Medium  : The QR Code can tolerate about 15% erroneous codewords";  break;
                        case ErrorCorrectionLevel::Value::kQuartile: return "Quartile: The QR Code can tolerate about 25% erroneous codewords";  break;
                        case ErrorCorrectionLevel::Value::kHigh    : return "High    : The QR Code can tolerate about 30% erroneous codewords";  break;
                    }
                }

                constexpr qrcodegen::QrCode::Ecc    ToQrCodeGen()  const
                {
                    switch (this->value_)
                    {
                        case ErrorCorrectionLevel::Value::kLow     : return qrcodegen::QrCode::Ecc::LOW;        break;
                        case ErrorCorrectionLevel::Value::kMedium  : return qrcodegen::QrCode::Ecc::MEDIUM;     break;
                        case ErrorCorrectionLevel::Value::kQuartile: return qrcodegen::QrCode::Ecc::QUARTILE;   break;
                        case ErrorCorrectionLevel::Value::kHigh    : return qrcodegen::QrCode::Ecc::HIGH;       break;
                    }
                }
        };
    };

    class IMPROC_EXPORTS QrCodeDrawer : public improc::BaseDrawer
    {
        private:
            static constexpr ZXing::ImageFormat     kImageFormat   = ZXing::ImageFormat::Lum;
            static constexpr int                    kImageDataType = CV_8UC1;
            improc::qrcode::ErrorCorrectionLevel    error_correction_level_;
            ZXing::DecodeHints                      hints_;
            
        public:
            QrCodeDrawer();
            QrCodeDrawer(const Json::Value& drawer_json);

            QrCodeDrawer&                           Load    (const Json::Value& drawer_json);
            cv::Mat                                 Draw    (const std::optional<std::string>& message = std::optional<std::string>());
            bool                                    Verify  (const cv::Mat& drawer_output, const std::optional<std::string>& message = std::optional<std::string>());
    };
}

#endif