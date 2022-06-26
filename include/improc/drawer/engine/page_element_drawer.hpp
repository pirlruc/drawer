#ifndef IMPROC_DRAWER_PAGE_ELEMENT_DRAWER_HPP
#define IMPROC_DRAWER_PAGE_ELEMENT_DRAWER_HPP

#include <improc/improc_defs.hpp>
#include <improc/exception.hpp>
#include <improc/drawer/engine/element_drawer.hpp>
#include <improc/drawer/parsers/corecv_json_parser.hpp>

#include <opencv2/core.hpp>
#include <json/json.h>

namespace improc 
{
    class IMPROC_EXPORTS PageElementDrawer : private improc::ElementDrawer
    {
        private:
            cv::Point                   top_left_;
            cv::Rect                    element_box_;
            bool                        static_;
            std::optional<std::string>  content_;

        public:
            PageElementDrawer();
            PageElementDrawer(const improc::DrawerFactory& factory, const Json::Value& page_element_drawer_json, const cv::Size& page_size);

            PageElementDrawer&          Load    (const improc::DrawerFactory& factory, const Json::Value& page_element_drawer_json, const cv::Size& page_size);
            PageElementDrawer&          Allocate();
            void                        Draw    (cv::Mat& page_image        , const std::optional<std::string>& message = std::optional<std::string>()) const;
            bool                        Verify  (const cv::Mat& page_image  , const std::optional<std::string>& message = std::optional<std::string>()) const;

            PageElementDrawer&          IncrementTopLeftBy(const cv::Point& increment_top_left, const cv::Size& page_size);

            static std::list<PageElementDrawer> IncrementTopLeftBy  ( std::list<PageElementDrawer>&& page_elements
                                                                    , const cv::Point& increment_top_left
                                                                    , const cv::Size& page_size );

            bool                        is_element_static() const;

        private:
            static cv::Point            ParsePoint          (const Json::Value& point_json, const cv::Size& page_size);
            static void                 ValidatePoint       (const cv::Point&   point     , const cv::Size& page_size);
            static bool                 IsPixelPositionValid(int pixel_position, int max_pixel_size);
    };
}

#endif