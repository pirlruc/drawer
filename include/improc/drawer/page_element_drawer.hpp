#ifndef IMPROC_DRAWER_PAGE_ELEMENT_DRAWER_HPP
#define IMPROC_DRAWER_PAGE_ELEMENT_DRAWER_HPP

#include <improc/improc_defs.hpp>
#include <improc/exception.hpp>
#include <improc/drawer/element_drawer.hpp>
#include <improc/drawer/parsers/corecv_json_parser.hpp>

#include <opencv2/core.hpp>
#include <json/json.h>

namespace improc 
{
    class IMPROC_EXPORTS PageElementDrawer : private improc::ElementDrawer
    {
        private:
            cv::Point           top_left_;

        public:
            PageElementDrawer();
            PageElementDrawer(const improc::DrawerFactory& factory, const Json::Value& page_element_drawer_json, const cv::Size& page_size);

            PageElementDrawer&  Load    (const improc::DrawerFactory& factory, const Json::Value& page_element_drawer_json, const cv::Size& page_size);
            void                Draw    (cv::Mat& page_image) const;

        private:
            static cv::Point    ParsePoint          (const Json::Value& point_json, const cv::Size& page_size);
            static bool         IsPixelPositionValid(int pixel_position, int max_pixel_size);
    };
}

#endif