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
    class IMPROC_EXPORTS ImageFileDrawer : public improc::BaseDrawer
    {
        private:
            static constexpr int    kImageReadMode = cv::IMREAD_GRAYSCALE;
            cv::Mat                 image_data_;

        public:
            ImageFileDrawer();
            ImageFileDrawer(const Json::Value& drawer_json);

            ImageFileDrawer&        Load(const Json::Value& drawer_json);
            cv::Mat                 Draw(const std::optional<std::string>& message = std::optional<std::string>());
    };
}

#endif