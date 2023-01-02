#ifndef IMPROC_DRAWER_PAGE_DRAWER_HPP
#define IMPROC_DRAWER_PAGE_DRAWER_HPP

#include <improc/improc_defs.hpp>
#include <improc/exception.hpp>
#include <improc/drawer/engine/page_element_drawer.hpp>
#include <improc/drawer/parsers/corecv_json_parser.hpp>

#include <opencv2/core.hpp>
#include <json/json.h>
#include <list>

namespace improc 
{
    class PageDrawer
    {
        protected:
            static constexpr int            kImageDataType = CV_8UC1;
            std::list<PageElementDrawer>    elements_;
            cv::Size                        page_size_;
            cv::Mat                         page_image_;

        public:
            PageDrawer();
            PageDrawer(const improc::DrawerFactory& factory, const Json::Value& page_drawer_json);

            PageDrawer&                     Load    (const improc::DrawerFactory& factory, const Json::Value& page_drawer_json);
            PageDrawer&                     Allocate();
            cv::Mat                         Draw    (const std::list<std::optional<std::string>>& context = std::list<std::optional<std::string>>());
            bool                            Verify  (const std::list<std::optional<std::string>>& context = std::list<std::optional<std::string>>());
            bool                            Verify  (const cv::Mat& page_image, const std::list<std::optional<std::string>>& context = std::list<std::optional<std::string>>());

            cv::Size                        get_page_size()     const;
            std::list<PageElementDrawer>    get_page_elements() const;

        private:
            PageDrawer&                     set_page_image(const cv::Mat& page_image);
    };
}

#endif