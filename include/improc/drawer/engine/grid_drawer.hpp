#ifndef IMPROC_DRAWER_GRID_DRAWER_HPP
#define IMPROC_DRAWER_GRID_DRAWER_HPP

#include <improc/improc_defs.hpp>
#include <improc/exception.hpp>
#include <improc/corecv/parsers/json_parser.hpp>
#include <improc/drawer/engine/page_drawer.hpp>

#include <opencv2/core.hpp>
#include <json/json.h>

namespace improc 
{
    /**
     * @brief Grid drawer object for drawer factory. 
     * This is an utility class to define a grid of page element drawers in a page.
     */
    class GridDrawer final: public improc::PageDrawer
    {
        public:
            GridDrawer();
            explicit GridDrawer(const improc::DrawerFactory& factory, const Json::Value& grid_drawer_json);

            GridDrawer&         Load    (const improc::DrawerFactory& factory, const Json::Value& grid_drawer_json);
            GridDrawer&         Allocate();

        private:
            static cv::Point    ParseGridNumber (const Json::Value& grid_number_json);
            static cv::Point    ParseGridSpacing(const Json::Value& grid_spacing_json);
            static inline bool  IsNumberValid   (int number);
            static inline bool  IsSpacingValid  (int spacing);
    };
}

#endif