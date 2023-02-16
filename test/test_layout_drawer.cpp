#include <gtest/gtest.h>

#include <improc_drawer_test_config.hpp>
#include <base_drawers_def.hpp>
#include <improc/drawer/layout_drawer.hpp>
#include <improc/infrastructure/filesystem/file.hpp>

TEST(LayoutDrawer,TestConstructor) {
    EXPECT_NO_THROW(improc::LayoutDrawer());
}

TEST(LayoutDrawer,TestEmptyDraw) {
    improc::LayoutDrawer drawer {};
    EXPECT_NO_THROW(drawer.Draw());
    EXPECT_TRUE(drawer.Verify());
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
    EXPECT_THROW(drawer.Allocate().Verify(),cv::Exception);
}

TEST(LayoutDrawer,TestDrawWithoutAllocation) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/layout_drawer_multiple_elem.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestLayoutDrawer>});
    improc::LayoutDrawer drawer = improc::LayoutDrawer(factory,json_content);
    std::list<std::optional<std::string>> context {};
    for (size_t idx = 0; idx < 8; idx++)
    {
        context.emplace_back("example");
        context.emplace_back();
        context.emplace_back();
    }
    context.emplace_back();
    context.emplace_back("example");
    context.emplace_back();
    EXPECT_THROW(drawer.Draw(context),cv::Exception);
    EXPECT_FALSE(drawer.Verify(context));
    EXPECT_THROW(drawer.Verify(cv::Mat(),context),improc::page_drawer_not_allocated);
}

TEST(LayoutDrawer,TestDrawWithAllocation) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/layout_drawer_multiple_elem.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestLayoutDrawer>});
    improc::LayoutDrawer drawer = improc::LayoutDrawer(factory,json_content);
    std::list<std::optional<std::string>> context {};
    for (size_t idx = 0; idx < 8; idx++)
    {
        context.emplace_back("example");
        context.emplace_back();
        context.emplace_back();
    }
    context.emplace_back();
    context.emplace_back("example");
    context.emplace_back();
    EXPECT_NO_THROW(drawer.Allocate().Draw(context));
    EXPECT_TRUE(drawer.Verify(context));
    EXPECT_THROW(drawer.Verify(cv::Mat(),context),improc::invalid_page_image);
    EXPECT_TRUE(drawer.Verify(drawer.Allocate().Draw(context),context));
}
