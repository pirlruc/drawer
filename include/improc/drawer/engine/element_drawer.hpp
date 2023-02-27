#ifndef IMPROC_DRAWER_ELEMENT_DRAWER_HPP
#define IMPROC_DRAWER_ELEMENT_DRAWER_HPP

#include <improc/improc_defs.hpp>
#include <improc/exception.hpp>
#include <improc/corecv/structures/rotation_type.hpp>
#include <improc/corecv/parsers/json_parser.hpp>
#include <improc/drawer/engine/base_drawer.hpp>

#include <opencv2/imgproc.hpp>
#include <json/json.h>
#include <optional>

namespace improc 
{
    /**
     * @brief Element drawer object for drawer factory. 
     * This class applies a rotation and a resizing operation to a base drawer. 
     */
    class IMPROC_API ElementDrawer
    {
        private:
            std::shared_ptr<BaseDrawer> drawer_;
            std::optional<RotationType> rotation_;
            std::optional<cv::Size>     size_;

        public:
            ElementDrawer();
            explicit ElementDrawer(const improc::DrawerFactory& factory, const Json::Value& element_drawer_json);

            ElementDrawer&              Load    (const improc::DrawerFactory& factory, const Json::Value& element_drawer_json);
            cv::Mat                     Draw    (const std::optional<std::string>& message = std::optional<std::string>()) const;
            bool                        Verify  (const cv::Mat& drawer_output, const std::optional<std::string>& message = std::optional<std::string>()) const;

        private:
            static unsigned int         GetScale(const cv::Size& current_size, const cv::Size& expected_size);
    };
}

#endif