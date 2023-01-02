#ifndef IMPROC_DRAWER_METRIC_PIXEL_JSON_CONVERSION_HPP
#define IMPROC_DRAWER_METRIC_PIXEL_JSON_CONVERSION_HPP

#include <improc/improc_defs.hpp>
#include <improc/exception.hpp>
#include <improc/drawer/logger_drawer.hpp>
#include <improc/drawer/engine/metric_pixel_converter.hpp>
#include <improc/drawer/parsers/corecv_json_parser.hpp>

#include <opencv2/core.hpp>
#include <json/json.h>

namespace improc 
{
    class IMPROC_API MetricPixelJsonConverter
    {
        public:
            MetricPixelJsonConverter();

            static Json::Value  Convert(const Json::Value& metric_json);
            static cv::Size     MetricSize2PixelSize  (const Json::Value& metric_size_json , const improc::MetricPixelConverter& pixel_converter);
            static cv::Point    MetricPoint2PixelPoint(const Json::Value& metric_point_json, const improc::MetricPixelConverter& pixel_converter);

        private:
            static void         ParseObject (const improc::MetricPixelConverter& pixel_converter, const std::string& object_name, Json::Value& object_json);
            static void         ParseArray  (const improc::MetricPixelConverter& pixel_converter, Json::Value& array_json);
    };
}

#endif