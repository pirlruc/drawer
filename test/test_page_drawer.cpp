#include <gtest/gtest.h>

#include <improc_drawer_test_config.hpp>

#include <improc/drawer/page_drawer.hpp>
#include <improc/infrastructure/filesystem/file.hpp>

class TestPageDrawer : public improc::BaseDrawer
{
    public:
        TestPageDrawer() {};
        TestPageDrawer(const Json::Value& drawer_json)
        {
            this->Load(drawer_json);
        }

        TestPageDrawer& Load(const Json::Value& drawer_json)
        {
            return (*this);
        }

        cv::Mat     Draw()  const
        {
            return cv::Mat::zeros(50,100,CV_8UC1);
        }
};

TEST(PageDrawer,TestConstructor) {
    EXPECT_NO_THROW(improc::PageDrawer());
}

TEST(PageDrawer,TestEmptyDraw) {
    improc::PageDrawer drawer {};
    EXPECT_NO_THROW(drawer.Draw());
}

TEST(PageDrawer,TestConstructorWithLoad) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageDrawer>});
    EXPECT_NO_THROW(improc::PageDrawer(factory,json_content));
}

TEST(PageDrawer,TestLoading) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageDrawer>});
    improc::PageDrawer drawer {};
    EXPECT_NO_THROW(drawer.Load(factory,json_content));
}

TEST(PageDrawer,TestNoPageSize) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_drawer_no_page_size.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageDrawer>});
    improc::PageDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),improc::file_processing_error);
}

TEST(PageDrawer,TestNoPageWidth) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_drawer_no_page_width.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageDrawer>});
    improc::PageDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),improc::file_processing_error);
}

TEST(PageDrawer,TestNoPageHeight) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_drawer_no_page_height.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageDrawer>});
    improc::PageDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),improc::file_processing_error);
}

TEST(PageDrawer,TestInvalidPageWidth) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_drawer_invalid_page_width.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageDrawer>});
    improc::PageDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),improc::file_processing_error);
}

TEST(PageDrawer,TestInvalidPageHeight) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_drawer_invalid_page_height.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageDrawer>});
    improc::PageDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),improc::file_processing_error);
}

TEST(PageDrawer,TestLoadSingle) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_drawer_single_elem.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageDrawer>});
    improc::PageDrawer drawer {};
    EXPECT_NO_THROW(drawer.Load(factory,json_content));
}

TEST(PageDrawer,TestLoadMultiple) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_drawer_multiple_elem.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageDrawer>});
    improc::PageDrawer drawer {};
    EXPECT_NO_THROW(drawer.Load(factory,json_content));
}

TEST(PageDrawer,TestDrawOutsidePage) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_drawer_outside_page.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageDrawer>});
    improc::PageDrawer drawer = improc::PageDrawer(factory,json_content);
    EXPECT_THROW(drawer.Draw(),cv::Exception);
}

TEST(PageDrawer,TestDraw) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_drawer_multiple_elem.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageDrawer>});
    improc::PageDrawer drawer = improc::PageDrawer(factory,json_content);
    EXPECT_NO_THROW(drawer.Draw());
}

TEST(PageDrawer,TestOverlayedDraw) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_drawer_multiple_elem_overlayed.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageDrawer>});
    improc::PageDrawer drawer = improc::PageDrawer(factory,json_content);
    EXPECT_NO_THROW(drawer.Draw());
}
