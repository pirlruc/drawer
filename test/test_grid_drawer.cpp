#include <gtest/gtest.h>

#include <improc_drawer_test_config.hpp>
#include <base_drawers_def.hpp>
#include <improc/drawer/engine/grid_drawer.hpp>
#include <improc/infrastructure/filesystem/file.hpp>

TEST(GridDrawer,TestConstructor) {
    EXPECT_NO_THROW(improc::GridDrawer());
}

TEST(GridDrawer,TestEmptyDraw) {
    improc::GridDrawer drawer {};
    EXPECT_NO_THROW(drawer.Draw());
    EXPECT_TRUE(drawer.Verify());
}

TEST(GridDrawer,TestConstructorWithLoad) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/grid_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestGridDrawer>});
    EXPECT_NO_THROW(improc::GridDrawer(factory,json_content));
}

TEST(GridDrawer,TestLoading) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/grid_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestGridDrawer>});
    improc::GridDrawer drawer {};
    EXPECT_NO_THROW(drawer.Load(factory,json_content));
}

TEST(GridDrawer,TestNoGridNumber) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/grid_drawer_no_number.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestGridDrawer>});
    improc::GridDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),improc::json_error);
}

TEST(GridDrawer,TestNoGridSpacing) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/grid_drawer_no_spacing.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestGridDrawer>});
    improc::GridDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),improc::json_error);
}

TEST(GridDrawer,TestNoCellContent) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/grid_drawer_no_cell.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestGridDrawer>});
    improc::GridDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),improc::json_error);
}

TEST(GridDrawer,TestInvalidNumberX) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/grid_drawer_invalid_number_x.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestGridDrawer>});
    improc::GridDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),improc::value_error);
}

TEST(GridDrawer,TestInvalidNumberY) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/grid_drawer_invalid_number_y.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestGridDrawer>});
    improc::GridDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),improc::value_error);
}

TEST(GridDrawer,TestInvalidSpacingX) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/grid_drawer_invalid_spacing_x.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestGridDrawer>});
    improc::GridDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),improc::value_error);
}

TEST(GridDrawer,TestInvalidSpacingY) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/grid_drawer_invalid_spacing_y.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestGridDrawer>});
    improc::GridDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),improc::value_error);
}

TEST(GridDrawer,TestDrawOutsidePage) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/grid_drawer_outside_page.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestGridDrawer>});
    improc::GridDrawer drawer = improc::GridDrawer(factory,json_content);
    EXPECT_THROW(drawer.Allocate().Draw(),cv::Exception);
    EXPECT_THROW(drawer.Allocate().Verify(),cv::Exception);
}

TEST(GridDrawer,TestDrawWithoutAllocation) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/grid_drawer_multiple_elem.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestGridDrawer>});
    improc::GridDrawer drawer = improc::GridDrawer(factory,json_content);
    std::list<std::optional<improc::DrawerVariant>> context {};
    for (size_t idx = 0; idx < drawer.get_page_elements().size()/3; idx++)
    {
        context.emplace_back();
        context.emplace_back("example");
        context.emplace_back();
    }
    EXPECT_THROW(drawer.Draw(context),improc::processing_flow_error);
    EXPECT_THROW(drawer.Verify(context),improc::processing_flow_error);
    EXPECT_THROW(drawer.Verify(cv::Mat(),context),improc::processing_flow_error);
}

TEST(GridDrawer,TestDrawWithAllocation) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/grid_drawer_multiple_elem.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestGridDrawer>});
    improc::GridDrawer drawer = improc::GridDrawer(factory,json_content);
    std::list<std::optional<improc::DrawerVariant>> context {};
    for (size_t idx = 0; idx < drawer.get_page_elements().size()/3; idx++)
    {
        context.emplace_back();
        context.emplace_back("example");
        context.emplace_back();
    }
    EXPECT_NO_THROW(drawer.Allocate().Draw(context));
    EXPECT_TRUE(drawer.Verify(context));
    EXPECT_THROW(drawer.Verify(cv::Mat(),context),improc::value_error);
    EXPECT_TRUE(drawer.Verify(drawer.Allocate().Draw(context),context));
}
