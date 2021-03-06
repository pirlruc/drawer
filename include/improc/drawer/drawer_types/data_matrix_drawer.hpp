#ifndef IMPROC_DRAWER_DATA_MATRIX_DRAWER_HPP
#define IMPROC_DRAWER_DATA_MATRIX_DRAWER_HPP

#include <improc/improc_defs.hpp>
#include <improc/drawer/engine/base_drawer.hpp>

#include <BitMatrix.h>
#include <BinaryBitmap.h>
#include <DecodeHints.h>
#include <DecoderResult.h>
#include <ThresholdBinarizer.h>
#include <datamatrix/DMWriter.h>
#include <datamatrix/DMSymbolShape.h>
#include <datamatrix/DMDecoder.h>
#include <codecvt>
#include <locale>

namespace improc 
{
    class IMPROC_EXPORTS DataMatrixDrawer : public improc::BaseDrawer
    {
        private:
            static constexpr ZXing::ImageFormat             kImageFormat        = ZXing::ImageFormat::Lum;
            static constexpr ZXing::DataMatrix::SymbolShape kSquareSymbolShape  = ZXing::DataMatrix::SymbolShape::SQUARE;
            static constexpr int                            kImageDataType      = CV_8UC1;
            static constexpr int                            kMargin             = 0;
            static constexpr int                            kMinWidth           = 0;
            static constexpr int                            kMinHeight          = 0;
            ZXing::DataMatrix::Writer                       writer_;
            ZXing::DecodeHints                              hints_;
            
        public:
            DataMatrixDrawer();
            DataMatrixDrawer(const Json::Value& drawer_json);

            DataMatrixDrawer&               Load    (const Json::Value& drawer_json);
            cv::Mat                         Draw    (const std::optional<std::string>& message = std::optional<std::string>());
            bool                            Verify  (const cv::Mat& drawer_output, const std::optional<std::string>& message = std::optional<std::string>());
    };
}

#endif