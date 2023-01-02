#ifndef IMPROC_DRAWER_BASE_DRAWER_HPP
#define IMPROC_DRAWER_BASE_DRAWER_HPP

#include <improc/improc_defs.hpp>
#include <improc/exception.hpp>
#include <improc/drawer/logger_drawer.hpp>
#include <improc/services/factory/factory_pattern.hpp>
#include <improc/infrastructure/parsers/json_parser.hpp>

#include <opencv2/core.hpp>
#include <json/json.h>
#include <functional>
#include <optional>

namespace improc 
{
    class DrawerFactory;

    // TODO: Add verify as a policy
    // TODO: Allow to use cv::MatExpr in the verify method
    class IMPROC_API BaseDrawer
    {
        public:
            BaseDrawer();
            BaseDrawer(const Json::Value& drawer_json);

            virtual BaseDrawer& Load    (const Json::Value& drawer_json) = 0;
            virtual cv::Mat     Draw    (const std::optional<std::string>& message = std::optional<std::string>()) = 0;
            virtual bool        Verify  (const cv::Mat& drawer_output, const std::optional<std::string>& message = std::optional<std::string>()) = 0;

            static std::shared_ptr<BaseDrawer> Create(const DrawerFactory& factory, const Json::Value& drawer_json);
    };

    class IMPROC_API DrawerFactory : public improc::FactoryPattern  < improc::BaseDrawer
                                                                        , std::string
                                                                        , std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)>>
    {
        public:
            DrawerFactory() : improc::FactoryPattern< improc::BaseDrawer
                                                    , std::string
                                                    , std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)>
                                                    > () {}
    };

    template<class DrawerType>
    IMPROC_API std::shared_ptr<improc::BaseDrawer> CreateDrawer(const Json::Value& drawer_json)
    {
        IMPROC_DRAWER_LOGGER_TRACE  ( "Creating shared pointer for drawer {}...", typeid(DrawerType).name() );
        std::shared_ptr<improc::BaseDrawer> drawer 
                {std::make_shared<DrawerType>(DrawerType(std::move(drawer_json)))};
        return drawer;
    }
}

#endif