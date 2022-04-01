#ifndef IMPROC_DRAWER_PAGE_DRAWER_HPP
#define IMPROC_DRAWER_PAGE_DRAWER_HPP

#include <improc/improc_defs.hpp>
#include <improc/exception.hpp>
#include <improc/drawer/page_element_drawer.hpp>
#include <improc/drawer/parsers/corecv_json_parser.hpp>

#include <opencv2/core.hpp>
#include <json/json.h>

namespace improc 
{
    class PageDrawer
    {
        private:
            std::vector<PageElementDrawer>  elements_;
            cv::Mat                         page_image_;

        public:
            PageDrawer();
            PageDrawer(const improc::DrawerFactory& factory, const Json::Value& page_drawer_json);

            PageDrawer&         Load    (const improc::DrawerFactory& factory, const Json::Value& page_drawer_json);
            PageDrawer&         Draw    ();

        private:
            static cv::Size     ParseSize    (const Json::Value& size_json);
            static bool         IsLengthValid(int length);
    };
}

#endif