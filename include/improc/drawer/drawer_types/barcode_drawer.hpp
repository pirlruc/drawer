#ifndef IMPROC_DRAWER_BARCODE_DRAWER_HPP
#define IMPROC_DRAWER_BARCODE_DRAWER_HPP

#include <improc/improc_defs.hpp>
#include <improc/drawer/engine/base_drawer.hpp>

#include <BitMatrix.h>
#include <BinaryBitmap.h>
#include <DecodeHints.h>
#include <Result.h>
#include <ThresholdBinarizer.h>
#include <oned/ODCode128Writer.h>
#include <oned/ODReader.h>
#include <codecvt>
#include <locale>

namespace improc 
{
    class IMPROC_EXPORTS BarcodeDrawer : public improc::BaseDrawer
    {
        private:
            static constexpr int                    kImageDataType = CV_8UC1;
            static constexpr int                    kMargin        = 0;
            static constexpr int                    kMinWidth      = 0;
            static constexpr int                    kMinHeight     = 10;
            static constexpr bool                   kTryHarder     = true;
            static constexpr bool                   kDoNotRotate   = false;
            static constexpr bool                   kNotPure       = false;
            static constexpr ZXing::BarcodeFormat   kBarcodeFormat = ZXing::BarcodeFormat::Code128;
            static constexpr ZXing::ImageFormat     kImageFormat   = ZXing::ImageFormat::Lum;
            ZXing::OneD::Code128Writer              writer_;
            ZXing::DecodeHints                      hints_;
            
        public:
            BarcodeDrawer();
            BarcodeDrawer(const Json::Value& drawer_json);

            BarcodeDrawer&                          Load    (const Json::Value& drawer_json);
            cv::Mat                                 Draw    (const std::optional<std::string>& message = std::optional<std::string>());
            bool                                    Verify  (const cv::Mat& drawer_output, const std::optional<std::string>& message = std::optional<std::string>());
    };
}

#endif