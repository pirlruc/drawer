#include <gtest/gtest.h>

#include <improc_drawer_test_config.hpp>
#include <base_drawers_def.hpp>
#include <improc/drawer/engine/page_element_drawer.hpp>
#include <improc/infrastructure/filesystem/file.hpp>

TEST(PageElementDrawer,TestConstructor) {
    EXPECT_NO_THROW(improc::PageElementDrawer());
    EXPECT_FALSE(improc::PageElementDrawer().is_element_static());
}

TEST(PageElementDrawer,TestEmptyDraw) {
    improc::PageElementDrawer drawer {};
    cv::Mat page = cv::Mat::zeros(200,100,CV_8UC1);
    EXPECT_THROW(drawer.Draw(page),improc::processing_flow_error);
    EXPECT_THROW(drawer.Verify(page),improc::processing_flow_error);
}

TEST(PageElementDrawer,TestConstructorWithLoad) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_element_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    cv::Mat page = cv::Mat::zeros(200,100,CV_8UC1);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageElemDrawer>});
    improc::PageElementDrawer drawer {factory,json_content,page.size()};
    EXPECT_FALSE(drawer.is_element_static());
}

TEST(PageElementDrawer,TestLoading) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_element_drawer_config_static.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    cv::Mat page = cv::Mat::zeros(200,100,CV_8UC1);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageElemDrawer>});
    improc::PageElementDrawer drawer {};
    EXPECT_NO_THROW(drawer.Load(factory,json_content,page.size()));
    EXPECT_TRUE(drawer.is_element_static());
}

TEST(PageElementDrawer,TestNoTopLeft) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_element_drawer_no_top_left.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    cv::Mat page = cv::Mat::zeros(200,100,CV_8UC1);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageElemDrawer>});
    improc::PageElementDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content,page.size()),improc::json_error);
}

TEST(PageElementDrawer,TestNoTopLeftX) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_element_drawer_no_top_left_x.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    cv::Mat page = cv::Mat::zeros(200,100,CV_8UC1);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageElemDrawer>});
    improc::PageElementDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content,page.size()),improc::json_error);
}

TEST(PageElementDrawer,TestNoTopLeftY) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_element_drawer_no_top_left_y.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    cv::Mat page = cv::Mat::zeros(200,100,CV_8UC1);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageElemDrawer>});
    improc::PageElementDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content,page.size()),improc::json_error);
}

TEST(PageElementDrawer,TestInvalidTopLeftX) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_element_drawer_invalid_x.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    cv::Mat page = cv::Mat::zeros(200,100,CV_8UC1);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageElemDrawer>});
    improc::PageElementDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content,page.size()),improc::value_error);
}

TEST(PageElementDrawer,TestInvalidTopLeftY) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_element_drawer_invalid_y.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    cv::Mat page = cv::Mat::zeros(200,100,CV_8UC1);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageElemDrawer>});
    improc::PageElementDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content,page.size()),improc::value_error);
}

TEST(PageElementDrawer,TestInvalidStaticDynamic) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_element_drawer_invalid_static_dynamic.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    cv::Mat page = cv::Mat::zeros(200,100,CV_8UC1);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageElemDrawer>});
    improc::PageElementDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content,page.size()),improc::json_error);
}

TEST(PageElementDrawer,TestDrawOutsidePage) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_element_drawer_outside_page.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    cv::Mat page = cv::Mat::zeros(200,100,CV_8UC1);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageElemDrawer>});
    improc::PageElementDrawer drawer = improc::PageElementDrawer(factory,json_content,page.size());
    EXPECT_THROW(drawer.Allocate().Draw(page),cv::Exception);
    EXPECT_THROW(drawer.Allocate().Verify(page),cv::Exception);
}

TEST(PageElementDrawer,TestDrawWithoutAllocation) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_element_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    cv::Mat page = cv::Mat::zeros(200,100,CV_8UC1);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageElemDrawer>});
    improc::PageElementDrawer drawer = improc::PageElementDrawer(factory,json_content,page.size());
    EXPECT_THROW(drawer.Draw(page),improc::processing_flow_error);
    EXPECT_THROW(drawer.Verify(page),improc::processing_flow_error);
}

TEST(PageElementDrawer,TestDrawWithAllocation) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_element_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    cv::Mat page = cv::Mat::zeros(200,100,CV_8UC1);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageElemDrawer>});
    improc::PageElementDrawer drawer = improc::PageElementDrawer(factory,json_content,page.size());
    drawer.Allocate().Draw(page);
    EXPECT_EQ(cv::countNonZero(page),50*100);
    EXPECT_TRUE(drawer.Verify(page));
}

TEST(PageElementDrawer,TestInvalidAllocationDrawerWithMessage) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_element_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    cv::Mat page = cv::Mat::zeros(200,100,CV_8UC1);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestDrawerWithMessage>});
    improc::PageElementDrawer drawer = improc::PageElementDrawer(factory,json_content,page.size());
    EXPECT_THROW(drawer.Allocate(),std::bad_optional_access);
}

TEST(PageElementDrawer,TestValidAllocationDrawerWithMessage) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_element_drawer_content_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    cv::Mat page = cv::Mat::zeros(200,100,CV_8UC1);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestDrawerWithMessage>});
    improc::PageElementDrawer drawer = improc::PageElementDrawer(factory,json_content,page.size());
    EXPECT_NO_THROW(drawer.Allocate());
}

TEST(PageElementDrawer,TestIncrementTopLeft) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_element_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    cv::Mat page_1 = cv::Mat::zeros(200,100,CV_8UC1);
    cv::Mat page_2 = cv::Mat::zeros(200,100,CV_8UC1);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageElemDrawer>});
    improc::PageElementDrawer drawer = improc::PageElementDrawer(factory,json_content,page_1.size());
    drawer.Allocate().Draw(page_1);
    EXPECT_EQ(page_1.at<uint8_t>(cv::Point(10,5)),255);
    EXPECT_TRUE(drawer.Verify(page_1));
    drawer.IncrementTopLeftBy(cv::Point(5,5),page_2.size());
    drawer.Allocate().Draw(page_2);
    EXPECT_EQ(page_2.at<uint8_t>(cv::Point(10,5)),0);
    EXPECT_EQ(page_2.at<uint8_t>(cv::Point(15,10)),255);
    EXPECT_TRUE(drawer.Verify(page_2));
}
