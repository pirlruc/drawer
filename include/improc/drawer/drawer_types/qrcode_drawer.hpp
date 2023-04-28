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
        /**
         * @brief QR-Code error correction methods and utilities
         */
        class IMPROC_API ErrorCorrectionLevel final
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
                explicit ErrorCorrectionLevel(const std::string& error_correction_level_str);

                /**
                 * @brief Construct a new improc::ErrorCorrectionLevel object
                 * 
                 * @param error_correction_level_value - error correction level value
                 */
                constexpr explicit                  ErrorCorrectionLevel(Value error_correction_level_value): value_(std::move(error_correction_level_value)) {}

                /**
                 * @brief Obtain error correction level value
                 */
                constexpr operator                  Value()     const {return this->value_;}

                /**
                 * @brief Obtain error correction level string description
                 */
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

                /**
                 * @brief Obtain error correction level qrcodegen code
                 */
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

    /**
     * @brief QR-Code drawer methods and utilities
     */
    class IMPROC_API QrCodeDrawer final: public improc::BaseDrawer
    {
        private:
            static constexpr ZXing::ImageFormat     kImageFormat   = ZXing::ImageFormat::Lum;
            improc::qrcode::ErrorCorrectionLevel    error_correction_level_;
            ZXing::DecodeHints                      hints_;
            
        public:
            QrCodeDrawer();
            explicit QrCodeDrawer(const Json::Value& drawer_json);

            QrCodeDrawer&                           Load    (const Json::Value& drawer_json);
            cv::Mat                                 Draw    (const std::optional<DrawerVariant>& message = std::optional<DrawerVariant>());
            bool                                    Verify  (const cv::Mat& drawer_output, const std::optional<DrawerVariant>& message = std::optional<DrawerVariant>());
    };
}

#endif