#ifndef IMPROC_DRAWER_METRIC_PIXEL_JSON_CONVERSION_HPP
#define IMPROC_DRAWER_METRIC_PIXEL_JSON_CONVERSION_HPP

#include <improc/improc_defs.hpp>
#include <improc/exception.hpp>
#include <improc/drawer/logger_drawer.hpp>
#include <improc/drawer/engine/metric_pixel_converter.hpp>
#include <improc/infrastructure/parsers/json_parser.hpp>

#include <opencv2/core.hpp>
#include <json/json.h>

namespace improc 
{
    class IMPROC_EXPORTS MetricPixelJsonConverter
    {
        public:
            MetricPixelJsonConverter();

            static Json::Value  Convert(const Json::Value& metric_json);

        private:
            static void         ParseObject (const improc::MetricPixelConverter& pixel_converter, const std::string& object_name, Json::Value& object_json);
            static void         ParseArray  (const improc::MetricPixelConverter& pixel_converter, Json::Value& array_json);

            // TODO: Remove these methods and add them to json parsers
            static cv::Point2d  ParsePoint  (const Json::Value& point_json);

        public:
            // TODO: Remove these methods and add them to json parsers
            static cv::Size2d   ParseSize   (const Json::Value& size_json);
    };
}

#endif