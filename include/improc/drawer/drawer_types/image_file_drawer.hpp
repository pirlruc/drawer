#ifndef IMPROC_DRAWER_IMAGE_FILE_DRAWER_HPP
#define IMPROC_DRAWER_IMAGE_FILE_DRAWER_HPP

#include <improc/improc_defs.hpp>
#include <improc/exception.hpp>
#include <improc/infrastructure/filesystem/file.hpp>
#include <improc/infrastructure/context/application_context.hpp>
#include <improc/drawer/engine/base_drawer.hpp>

#include <opencv2/imgcodecs.hpp>
#include <json/json.h>

namespace improc 
{
    /**
     * @brief Image file drawer methods and utilities
     */
    class IMPROC_API ImageFileDrawer final: public improc::BaseDrawer
    {
        private:
            static constexpr int    kImageReadMode = cv::IMREAD_GRAYSCALE;
            static constexpr int    kImageNorm     = cv::NORM_L1;
            cv::Mat                 image_data_;

        public:
            ImageFileDrawer();
            explicit ImageFileDrawer(const Json::Value& drawer_json);

            ImageFileDrawer&        Load    (const Json::Value& drawer_json);
            cv::Mat                 Draw    (const std::optional<DrawerVariant>& message = std::optional<DrawerVariant>());
            bool                    Verify  (const cv::Mat& drawer_output, const std::optional<DrawerVariant>& message = std::optional<DrawerVariant>());
    };
}

#endif