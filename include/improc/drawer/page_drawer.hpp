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
        protected:
            std::vector<PageElementDrawer>  elements_;
            cv::Size                        page_size_;
            cv::Mat                         page_image_;

        public:
            PageDrawer();
            PageDrawer(const improc::DrawerFactory& factory, const Json::Value& page_drawer_json);

            PageDrawer&                     Load    (const improc::DrawerFactory& factory, const Json::Value& page_drawer_json);
            PageDrawer&                     Allocate();
            PageDrawer&                     Draw    ();

            cv::Size                        get_page_size()     const;
            std::vector<PageElementDrawer>  get_page_elements() const;
    };
}

#endif