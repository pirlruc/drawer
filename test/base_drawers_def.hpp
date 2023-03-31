#ifndef IMPROC_TEST_BASE_DRAWER_DEFINITIONS_HPP
#define IMPROC_TEST_BASE_DRAWER_DEFINITIONS_HPP

#include <improc/drawer/engine/page_drawer.hpp>

class TestPageDrawer : public improc::BaseDrawer
{
    public:
        TestPageDrawer() {};
        explicit TestPageDrawer(const Json::Value& drawer_json)
        {
            this->Load(drawer_json);
        }

        TestPageDrawer& Load(const Json::Value& drawer_json)
        {
            return (*this);
        }

        cv::Mat     Draw(const std::optional<std::string>& message = std::optional<std::string>()) 
        {
            return cv::Mat::zeros(50,100,CV_8UC1);
        }

        bool        Verify(const cv::Mat& drawer_output, const std::optional<std::string>& message = std::optional<std::string>())
        {
            return drawer_output.rows == 50 && drawer_output.cols == 100;
        }
};

class TestPageElemDrawer : public improc::BaseDrawer
{
    public:
        TestPageElemDrawer() {};
        explicit TestPageElemDrawer(const Json::Value& drawer_json)
        {
            this->Load(drawer_json);
        }

        TestPageElemDrawer& Load(const Json::Value& drawer_json)
        {
            return (*this);
        }

        cv::Mat     Draw(const std::optional<std::string>& message = std::optional<std::string>())
        {
            return 255 * cv::Mat::ones(50,100,CV_8UC1);
        }

        bool        Verify(const cv::Mat& drawer_output, const std::optional<std::string>& message = std::optional<std::string>())
        {
            return drawer_output.rows == 50 && drawer_output.cols == 100;
        }
};

class TestDrawer : public improc::BaseDrawer
{
    public:
        TestDrawer() {};
        explicit TestDrawer(const Json::Value& drawer_json)
        {
            this->Load(drawer_json);
        }

        TestDrawer& Load(const Json::Value& drawer_json)
        {
            return (*this);
        }

        cv::Mat     Draw(const std::optional<std::string>& message = std::optional<std::string>()) 
        {
            return cv::Mat::ones(10,20,CV_8UC1);
        }

        bool        Verify(const cv::Mat& drawer_output, const std::optional<std::string>& message = std::optional<std::string>())
        {
            return drawer_output.rows == 10 && drawer_output.cols == 20;
        }
};

class TestDrawerWithMessage : public improc::BaseDrawer
{
    public:
        TestDrawerWithMessage() {};
        explicit TestDrawerWithMessage(const Json::Value& drawer_json)
        {
            this->Load(drawer_json);
        }

        TestDrawerWithMessage& Load(const Json::Value& drawer_json)
        {
            return (*this);
        }

        cv::Mat     Draw(const std::optional<std::string>& message = std::optional<std::string>()) 
        {
            std::cout << message.value() << std::endl;
            return cv::Mat::ones(10,20,CV_8UC1);
        }

        bool        Verify(const cv::Mat& drawer_output, const std::optional<std::string>& message = std::optional<std::string>())
        {
            return drawer_output.rows == 10 && drawer_output.cols == 20;
        }
};

typedef TestPageDrawer  TestPageDrawer;
typedef TestPageDrawer  TestGridDrawer;
typedef TestPageDrawer  TestLayoutDrawer;

#endif