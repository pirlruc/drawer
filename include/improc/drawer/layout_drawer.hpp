#ifndef IMPROC_DRAWER_LAYOUT_DRAWER_HPP
#define IMPROC_DRAWER_LAYOUT_DRAWER_HPP

#include <improc/improc_defs.hpp>
#include <improc/exception.hpp>
#include <improc/drawer/engine/page_drawer.hpp>
#include <improc/drawer/engine/grid_drawer.hpp>
#include <improc/drawer/engine/page_drawer_type.hpp>
#include <improc/drawer/parsers/corecv_json_parser.hpp>

#include <opencv2/core.hpp>
#include <json/json.h>

namespace improc 
{
    class LayoutDrawer : protected improc::PageDrawer
    {
        public:
            LayoutDrawer();
            LayoutDrawer(const improc::DrawerFactory& factory, const Json::Value& grid_drawer_json);

            LayoutDrawer&       Load    (const improc::DrawerFactory& factory, const Json::Value& layout_drawer_json);
            LayoutDrawer&       Allocate();
            cv::Mat             Draw    (const std::list<std::optional<std::string>>& context = std::list<std::optional<std::string>>());
            bool                Verify  (const std::list<std::optional<std::string>>& context = std::list<std::optional<std::string>>());
            bool                Verify  (const cv::Mat& page_image, const std::list<std::optional<std::string>>& context = std::list<std::optional<std::string>>());

            cv::Size            get_page_size()     const;

        private:
            LayoutDrawer&       ParsePageTypeDrawer (const improc::DrawerFactory& factory, const Json::Value& page_drawer_type_json);
            LayoutDrawer&       CorrectLayoutSize   (const cv::Point& top_left, const cv::Size& page_drawer_size);
            static cv::Point    ParsePoint          (const Json::Value& point_json);
            static bool         IsPixelPositionValid(int pixel_position);
    };
}

#endif