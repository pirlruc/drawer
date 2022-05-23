#include <gtest/gtest.h>

#include <improc_drawer_test_config.hpp>

#include <improc/drawer/layout_drawer.hpp>
#include <improc/infrastructure/filesystem/file.hpp>

class TestLayoutDrawer : public improc::BaseDrawer
{
    public:
        TestLayoutDrawer() {};
        TestLayoutDrawer(const Json::Value& drawer_json)
        {
            this->Load(drawer_json);
        }

        TestLayoutDrawer& Load(const Json::Value& drawer_json)
        {
            return (*this);
        }

        cv::Mat     Draw(const std::optional<std::string>& message = std::optional<std::string>())  const
        {
            return cv::Mat::zeros(50,100,CV_8UC1);
        }
};

TEST(LayoutDrawer,TestConstructor) {
    EXPECT_NO_THROW(improc::LayoutDrawer());
}

TEST(LayoutDrawer,TestEmptyDraw) {
    improc::LayoutDrawer drawer {};
    EXPECT_NO_THROW(drawer.Draw());
}

TEST(LayoutDrawer,TestConstructorWithLoad) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/layout_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestLayoutDrawer>});
    EXPECT_NO_THROW(improc::LayoutDrawer(factory,json_content));
}

TEST(LayoutDrawer,TestLoading) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/layout_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestLayoutDrawer>});
    improc::LayoutDrawer drawer {};
    EXPECT_NO_THROW(drawer.Load(factory,json_content));
}

TEST(LayoutDrawer,TestNoPageDrawerType) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/layout_drawer_no_page_drawer_type.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestLayoutDrawer>});
    improc::LayoutDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),improc::file_processing_error);
}

TEST(LayoutDrawer,TestNoTopLeft) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/layout_drawer_no_top_left.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestLayoutDrawer>});
    improc::LayoutDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),improc::file_processing_error);
}

TEST(LayoutDrawer,TestInvalidPointX) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/layout_drawer_invalid_top_left_x.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestLayoutDrawer>});
    improc::LayoutDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),improc::file_processing_error);
}

TEST(LayoutDrawer,TestInvalidPointY) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/layout_drawer_invalid_top_left_y.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestLayoutDrawer>});
    improc::LayoutDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),improc::file_processing_error);
}

TEST(LayoutDrawer,TestLoadSingle) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/layout_drawer_single_elem.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestLayoutDrawer>});
    improc::LayoutDrawer drawer {};
    EXPECT_NO_THROW(drawer.Load(factory,json_content));
}

TEST(LayoutDrawer,TestDrawOutsidePage) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/layout_drawer_outside_page.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestLayoutDrawer>});
    improc::LayoutDrawer drawer = improc::LayoutDrawer(factory,json_content);
    EXPECT_THROW(drawer.Allocate().Draw(),cv::Exception);
}

TEST(LayoutDrawer,TestDrawWithoutAllocation) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/layout_drawer_multiple_elem.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestLayoutDrawer>});
    improc::LayoutDrawer drawer = improc::LayoutDrawer(factory,json_content);
    EXPECT_THROW(drawer.Draw(),cv::Exception);
}

TEST(LayoutDrawer,TestDrawWithAllocation) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/layout_drawer_multiple_elem.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestLayoutDrawer>});
    improc::LayoutDrawer drawer = improc::LayoutDrawer(factory,json_content);
    EXPECT_NO_THROW(drawer.Allocate().Draw());
}
