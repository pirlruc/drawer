#include <gtest/gtest.h>

#include <improc_drawer_test_config.hpp>

#include <improc/drawer/page_element_drawer.hpp>
#include <improc/infrastructure/filesystem/file.hpp>

class TestPageElemDrawer : public improc::BaseDrawer
{
    public:
        TestPageElemDrawer() {};
        TestPageElemDrawer(const Json::Value& drawer_json)
        {
            this->Load(drawer_json);
        }

        TestPageElemDrawer& Load(const Json::Value& drawer_json)
        {
            return (*this);
        }

        cv::Mat     Draw()  const
        {
            return 255 * cv::Mat::ones(50,100,CV_8UC1);
        }
};

TEST(PageElementDrawer,TestConstructor) {
    EXPECT_NO_THROW(improc::PageElementDrawer());
}

TEST(PageElementDrawer,TestEmptyDraw) {
    improc::PageElementDrawer drawer {};
    cv::Mat page = cv::Mat::zeros(200,100,CV_8UC1);
    EXPECT_THROW(drawer.Draw(page),improc::drawer_not_defined);
}

TEST(PageElementDrawer,TestConstructorWithLoad) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_element_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    cv::Mat page = cv::Mat::zeros(200,100,CV_8UC1);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageElemDrawer>});
    EXPECT_NO_THROW(improc::PageElementDrawer(factory,json_content,page.size()));
}

TEST(PageElementDrawer,TestLoading) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_element_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    cv::Mat page = cv::Mat::zeros(200,100,CV_8UC1);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageElemDrawer>});
    improc::PageElementDrawer drawer {};
    EXPECT_NO_THROW(drawer.Load(factory,json_content,page.size()));
}

TEST(PageElementDrawer,TestNoTopLeft) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_element_drawer_no_top_left.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    cv::Mat page = cv::Mat::zeros(200,100,CV_8UC1);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageElemDrawer>});
    improc::PageElementDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content,page.size()),improc::file_processing_error);
}

TEST(PageElementDrawer,TestNoTopLeftX) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_element_drawer_no_top_left_x.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    cv::Mat page = cv::Mat::zeros(200,100,CV_8UC1);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageElemDrawer>});
    improc::PageElementDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content,page.size()),improc::file_processing_error);
}

TEST(PageElementDrawer,TestNoTopLeftY) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_element_drawer_no_top_left_y.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    cv::Mat page = cv::Mat::zeros(200,100,CV_8UC1);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageElemDrawer>});
    improc::PageElementDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content,page.size()),improc::file_processing_error);
}

TEST(PageElementDrawer,TestInvalidTopLeftX) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_element_drawer_invalid_x.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    cv::Mat page = cv::Mat::zeros(200,100,CV_8UC1);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageElemDrawer>});
    improc::PageElementDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content,page.size()),improc::file_processing_error);
}

TEST(PageElementDrawer,TestInvalidTopLeftY) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_element_drawer_invalid_y.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    cv::Mat page = cv::Mat::zeros(200,100,CV_8UC1);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageElemDrawer>});
    improc::PageElementDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content,page.size()),improc::file_processing_error);
}

TEST(PageElementDrawer,TestDrawOutsidePage) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_element_drawer_outside_page.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    cv::Mat page = cv::Mat::zeros(200,100,CV_8UC1);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageElemDrawer>});
    improc::PageElementDrawer drawer = improc::PageElementDrawer(factory,json_content,page.size());
    EXPECT_THROW(drawer.Draw(page),cv::Exception);
}

TEST(PageElementDrawer,TestDraw) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_element_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    cv::Mat page = cv::Mat::zeros(200,100,CV_8UC1);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageElemDrawer>});
    improc::PageElementDrawer drawer = improc::PageElementDrawer(factory,json_content,page.size());
    drawer.Draw(page);
    EXPECT_EQ(cv::countNonZero(page),50*100);
}
