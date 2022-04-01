#ifndef IMPROC_DRAWER_ELEMENT_DRAWER_HPP
#define IMPROC_DRAWER_ELEMENT_DRAWER_HPP

#include <improc/improc_defs.hpp>
#include <improc/exception.hpp>
#include <improc/drawer/rotation_type.hpp>
#include <improc/drawer/base_drawer.hpp>

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
            std::optional<unsigned int> scale_;

        public:
            ElementDrawer();
            ElementDrawer(const improc::DrawerFactory& factory, const Json::Value& element_drawer_json);

            ElementDrawer&              Load(const improc::DrawerFactory& factory, const Json::Value& element_drawer_json);
            cv::Mat                     Draw() const;

        private:
            static unsigned int         ParseScale(const Json::Value& scale_json);
    };
}

#endif