#include <gtest/gtest.h>

#include <improc_drawer_test_config.hpp>

#include <improc/drawer/grid_drawer.hpp>
#include <improc/infrastructure/filesystem/file.hpp>

class TestGridDrawer : public improc::BaseDrawer
{
    public:
        TestGridDrawer() {};
        TestGridDrawer(const Json::Value& drawer_json)
        {
            this->Load(drawer_json);
        }

        TestGridDrawer& Load(const Json::Value& drawer_json)
        {
            return (*this);
        }

        cv::Mat     Draw()  const
        {
            return cv::Mat::zeros(50,100,CV_8UC1);
        }
};

TEST(GridDrawer,TestConstructor) {
    EXPECT_NO_THROW(improc::GridDrawer());
}

TEST(GridDrawer,TestEmptyDraw) {
    improc::GridDrawer drawer {};
    EXPECT_NO_THROW(drawer.Draw());
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
    EXPECT_THROW(drawer.Load(factory,json_content),improc::file_processing_error);
}

TEST(GridDrawer,TestNoGridSpacing) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/grid_drawer_no_spacing.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestGridDrawer>});
    improc::GridDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),improc::file_processing_error);
}

TEST(GridDrawer,TestNoCellContent) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/grid_drawer_no_cell.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestGridDrawer>});
    improc::GridDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),improc::file_processing_error);
}

TEST(GridDrawer,TestInvalidNumberX) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/grid_drawer_invalid_number_x.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestGridDrawer>});
    improc::GridDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),improc::file_processing_error);
}

TEST(GridDrawer,TestInvalidNumberY) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/grid_drawer_invalid_number_y.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestGridDrawer>});
    improc::GridDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),improc::file_processing_error);
}

TEST(GridDrawer,TestInvalidSpacingX) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/grid_drawer_invalid_spacing_x.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestGridDrawer>});
    improc::GridDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),improc::file_processing_error);
}

TEST(GridDrawer,TestInvalidSpacingY) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/grid_drawer_invalid_spacing_y.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestGridDrawer>});
    improc::GridDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),improc::file_processing_error);
}

TEST(GridDrawer,TestDrawOutsidePage) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/grid_drawer_outside_page.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestGridDrawer>});
    improc::GridDrawer drawer = improc::GridDrawer(factory,json_content);
    EXPECT_THROW(drawer.Allocate().Draw(),cv::Exception);
}

TEST(GridDrawer,TestDrawWithoutAllocation) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/grid_drawer_multiple_elem.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestGridDrawer>});
    improc::GridDrawer drawer = improc::GridDrawer(factory,json_content);
    EXPECT_THROW(drawer.Draw(),cv::Exception);
}

TEST(GridDrawer,TestDrawWithAllocation) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/grid_drawer_multiple_elem.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestGridDrawer>});
    improc::GridDrawer drawer = improc::GridDrawer(factory,json_content);
    EXPECT_NO_THROW(drawer.Allocate().Draw());
}
