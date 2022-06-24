#ifndef IMPROC_DRAWER_TEXT_DRAWER_HPP
#define IMPROC_DRAWER_TEXT_DRAWER_HPP

#include <improc/improc_defs.hpp>
#include <improc/drawer/engine/base_drawer.hpp>
#include <improc/drawer/engine/metric_pixel_converter.hpp>
#include <improc/drawer/engine/metric_pixel_json_converter.hpp>
#include <improc/infrastructure/filesystem/file.hpp>
#include <improc/infrastructure/context/application_context.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace improc 
{
    class IMPROC_EXPORTS TextDrawer : public improc::BaseDrawer
    {
        private:
            static constexpr int            kImageDataType         = CV_8UC1;
            static constexpr unsigned int   kFontSizePointFraction = 64; 
            static constexpr unsigned int   kLoadFirstFont         = 0; 
            bool                            font_loaded_;
            FT_Library                      library_;
            FT_Face                         face_;
            cv::Size                        image_text_size_;
            unsigned int                    printing_resolution_;
            unsigned int                    font_size_;
            
        public:
            TextDrawer();
            TextDrawer(const Json::Value& drawer_json);

            TextDrawer&                     Load    (const Json::Value& drawer_json);
            cv::Mat                         Draw    (const std::optional<std::string>& message = std::optional<std::string>());
            bool                            Verify  (const cv::Mat& drawer_output, const std::optional<std::string>& message = std::optional<std::string>()) {return true;};

        private:
            static cv::Size                 ParseMetricSize(const Json::Value& size_json, const improc::MetricPixelConverter& pixel_converter);
            cv::Mat                         Bitmap2Mat(const FT_Bitmap& char_bitmap) const;
    };
}

#endif