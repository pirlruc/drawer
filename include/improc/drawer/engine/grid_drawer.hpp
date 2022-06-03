#ifndef IMPROC_DRAWER_GRID_DRAWER_HPP
#define IMPROC_DRAWER_GRID_DRAWER_HPP

#include <improc/improc_defs.hpp>
#include <improc/exception.hpp>
#include <improc/drawer/engine/page_drawer.hpp>
#include <improc/drawer/parsers/corecv_json_parser.hpp>

#include <opencv2/core.hpp>
#include <json/json.h>

namespace improc 
{
    class GridDrawer : public improc::PageDrawer
    {
        public:
            GridDrawer();
            GridDrawer(const improc::DrawerFactory& factory, const Json::Value& grid_drawer_json);

            GridDrawer&         Load    (const improc::DrawerFactory& factory, const Json::Value& grid_drawer_json);
            GridDrawer&         Allocate();
            cv::Mat             Draw    (const std::list<std::optional<std::string>>& context = std::list<std::optional<std::string>>());

        private:
            static cv::Point    ParseGridNumber (const Json::Value& grid_number_json);
            static cv::Point    ParseGridSpacing(const Json::Value& grid_spacing_json);
            static bool         IsNumberValid   (int number);
            static bool         IsSpacingValid  (int spacing);
    };
}

#endif