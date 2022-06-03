#ifndef IMPROC_DRAWER_ELEMENT_DRAWER_HPP
#define IMPROC_DRAWER_ELEMENT_DRAWER_HPP

#include <improc/improc_defs.hpp>
#include <improc/exception.hpp>
#include <improc/drawer/parsers/rotation_type.hpp>
#include <improc/drawer/engine/base_drawer.hpp>
#include <improc/drawer/parsers/corecv_json_parser.hpp>

#include <opencv2/imgproc.hpp>
#include <json/json.h>
#include <optional>

namespace improc 
{
    class IMPROC_EXPORTS ElementDrawer
    {
        private:
            std::shared_ptr<BaseDrawer> drawer_;
            std::optional<RotationType> rotation_;
            std::optional<cv::Size>     size_;

        public:
            ElementDrawer();
            ElementDrawer(const improc::DrawerFactory& factory, const Json::Value& element_drawer_json);

            ElementDrawer&              Load(const improc::DrawerFactory& factory, const Json::Value& element_drawer_json);
            cv::Mat                     Draw(const std::optional<std::string>& message = std::optional<std::string>()) const;

        private:
            static unsigned int         GetScale(const cv::Size& current_size, const cv::Size& expected_size);

        public:
            // TODO: Move ParseSize and IsLengthValid to a common parsing structure
            static cv::Size             ParseSize    (const Json::Value& size_json);
            static bool                 IsLengthValid(int length);
    };
}

#endif