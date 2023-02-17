#include <gtest/gtest.h>

#include <improc_drawer_test_config.hpp>
#include <base_drawers_def.hpp>
#include <improc/drawer/engine/page_drawer.hpp>
#include <improc/infrastructure/filesystem/file.hpp>

TEST(PageDrawer,TestConstructor) {
    EXPECT_NO_THROW(improc::PageDrawer());
}

TEST(PageDrawer,TestEmptyDraw) {
    improc::PageDrawer drawer {};
    EXPECT_NO_THROW(drawer.Draw());
    EXPECT_TRUE(drawer.Verify());
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
    EXPECT_THROW(drawer.Load(factory,json_content),improc::json_error);
}

TEST(PageDrawer,TestNoPageWidth) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_drawer_no_page_width.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageDrawer>});
    improc::PageDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),improc::json_error);
}

TEST(PageDrawer,TestNoPageHeight) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_drawer_no_page_height.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageDrawer>});
    improc::PageDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),improc::json_error);
}

TEST(PageDrawer,TestInvalidPageWidth) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_drawer_invalid_page_width.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageDrawer>});
    improc::PageDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),improc::value_error);
}

TEST(PageDrawer,TestInvalidPageHeight) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_drawer_invalid_page_height.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageDrawer>});
    improc::PageDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),improc::value_error);
}

TEST(PageDrawer,TestLoadSingle) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_drawer_single_elem.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageDrawer>});
    improc::PageDrawer drawer {};
    EXPECT_NO_THROW(drawer.Load(factory,json_content));
}

TEST(PageDrawer,TestDrawWithoutContext) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_drawer_single_elem.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageDrawer>});
    improc::PageDrawer drawer {};
    drawer.Load(factory,json_content);
    EXPECT_THROW(drawer.Allocate().Draw(),improc::context_elem_diff_page_elem);
    EXPECT_THROW(drawer.Allocate().Verify(),improc::context_elem_diff_page_elem);
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
    EXPECT_THROW(drawer.Allocate().Draw(),cv::Exception);
    EXPECT_THROW(drawer.Allocate().Verify(),cv::Exception);
}

TEST(PageDrawer,TestDrawWithoutAllocation) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_drawer_multiple_elem.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageDrawer>});
    improc::PageDrawer drawer = improc::PageDrawer(factory,json_content);
    std::list<std::optional<std::string>> context {};
    context.push_back("test_a");
    context.emplace_back();
    context.emplace_back();    
    EXPECT_THROW(drawer.Draw(context),cv::Exception);
    EXPECT_FALSE(drawer.Verify(context));
    EXPECT_THROW(drawer.Verify(cv::Mat(),context),improc::page_drawer_not_allocated);
}

TEST(PageDrawer,TestDrawWithAllocation) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_drawer_multiple_elem.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageDrawer>});
    improc::PageDrawer drawer = improc::PageDrawer(factory,json_content);
    std::list<std::optional<std::string>> context {};
    context.push_back("test_a");
    context.emplace_back();
    context.emplace_back();    
    EXPECT_NO_THROW(drawer.Allocate().Draw(context));
    EXPECT_TRUE(drawer.Verify(context));
    EXPECT_THROW(drawer.Verify(cv::Mat(),context),improc::invalid_page_image);
    EXPECT_TRUE(drawer.Verify(drawer.Allocate().Draw(context),context));
}

TEST(PageDrawer,TestOverlayedDraw) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/page_drawer_multiple_elem_overlayed.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestPageDrawer>});
    improc::PageDrawer drawer = improc::PageDrawer(factory,json_content);
    std::list<std::optional<std::string>> context {};
    context.push_back("test_a");
    context.emplace_back();
    context.emplace_back();    
    EXPECT_NO_THROW(drawer.Allocate().Draw(context));
    EXPECT_TRUE(drawer.Verify(context));
}
