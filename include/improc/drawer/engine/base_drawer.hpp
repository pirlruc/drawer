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

    /**
     * @brief Base drawer object for drawer factory
     */
    // TODO: Allow to use cv::MatExpr in the verify method
    class IMPROC_API BaseDrawer
    {
        public:
            static constexpr int    kImageDataType = CV_8UC1;
            static constexpr int    kBlackValue    = 0;
            static constexpr int    kWhiteValue    = 255;

        public:
            BaseDrawer();
            explicit BaseDrawer(const Json::Value& drawer_json);

            virtual BaseDrawer&     Load    (const Json::Value& drawer_json) = 0;
            virtual cv::Mat         Draw    (const std::optional<std::string>& message = std::optional<std::string>()) = 0;
            virtual bool            Verify  (const cv::Mat& drawer_output, const std::optional<std::string>& message = std::optional<std::string>()) = 0;

            static std::shared_ptr<BaseDrawer> Create(const DrawerFactory& factory, const Json::Value& drawer_json);
    };

    /**
     * @brief Drawer factory methods and utilities
     */
    class IMPROC_API DrawerFactory final: public improc::FactoryPattern < improc::BaseDrawer
                                                                        , std::string
                                                                        , std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)>>
    {
        public:
            /**
             * @brief Construct a new improc::DrawerFactory object
             */
            DrawerFactory() : improc::FactoryPattern< improc::BaseDrawer
                                                    , std::string
                                                    , std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)>
                                                    > () {}
    };

    /**
     * @brief Create drawer object from json
     * @param drawer_json - configuration json for drawer
     * @return std::shared_ptr<improc::BaseDrawer> - drawer object pointer
     */
    //TODO: Create template method to create objects for factories
    template<class DrawerType>
    IMPROC_API std::shared_ptr<improc::BaseDrawer> CreateDrawer(const Json::Value& drawer_json)
    {
        IMPROC_DRAWER_LOGGER_TRACE("Creating shared pointer for drawer {}...",typeid(DrawerType).name());
        std::shared_ptr<improc::BaseDrawer> drawer 
                {std::make_shared<DrawerType>(DrawerType(std::move(drawer_json)))};
        return drawer;
    }
}

#endif