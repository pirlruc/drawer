#ifndef IMPROC_DRAWER_DATA_MATRIX_DRAWER_HPP
#define IMPROC_DRAWER_DATA_MATRIX_DRAWER_HPP

#include <improc/improc_defs.hpp>
#include <improc/drawer/engine/base_drawer.hpp>

#include <ZXing/BitMatrix.h>
#include <BinaryBitmap.h>
#include <DecoderResult.h>
#include <ThresholdBinarizer.h>
#include <datamatrix/DMWriter.h>
#include <datamatrix/DMSymbolShape.h>
#include <datamatrix/DMDecoder.h>
#include <codecvt>
#include <locale>

namespace improc 
{
    /**
     * @brief Data matrix drawer methods and utilities
     */
    class IMPROC_API DataMatrixDrawer final: public improc::BaseDrawer
    {
        private:
            static constexpr ZXing::ImageFormat             kImageFormat        = ZXing::ImageFormat::Lum;
            static constexpr ZXing::DataMatrix::SymbolShape kSquareSymbolShape  = ZXing::DataMatrix::SymbolShape::SQUARE;
            static constexpr int                            kMargin             = 0;
            static constexpr int                            kMinWidth           = 0;
            static constexpr int                            kMinHeight          = 0;
            ZXing::DataMatrix::Writer                       writer_;
            
        public:
            DataMatrixDrawer();
            explicit DataMatrixDrawer(const Json::Value& drawer_json);

            DataMatrixDrawer&               Load    (const Json::Value& drawer_json);
            cv::Mat                         Draw    (const std::optional<DrawerVariant>& message = std::optional<DrawerVariant>());
            bool                            Verify  (const cv::Mat& drawer_output, const std::optional<DrawerVariant>& message = std::optional<DrawerVariant>());
    };
}

#endif