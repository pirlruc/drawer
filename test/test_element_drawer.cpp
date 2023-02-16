#include <gtest/gtest.h>

#include <improc_drawer_test_config.hpp>
#include <base_drawers_def.hpp>
#include <improc/drawer/engine/element_drawer.hpp>
#include <improc/infrastructure/filesystem/file.hpp>

TEST(ElementDrawer,TestConstructor) {
    EXPECT_NO_THROW(improc::ElementDrawer());
}

TEST(ElementDrawer,TestEmptyDraw) {
    improc::ElementDrawer drawer {};
    EXPECT_THROW(drawer.Draw(),improc::drawer_not_defined);
    EXPECT_THROW(drawer.Verify(cv::Mat()),improc::drawer_not_defined);
}

TEST(ElementDrawer,TestConstructorWithLoad) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("increment",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestDrawer>});
    EXPECT_NO_THROW(improc::ElementDrawer(factory,json_content));
}

TEST(ElementDrawer,TestLoading) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("increment",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestDrawer>});
    improc::ElementDrawer drawer {};
    EXPECT_NO_THROW(drawer.Load(factory,json_content));
}

TEST(ElementDrawer,TestInvalidRotation) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/element_drawer_invalid_rotation.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestDrawer>});
    improc::ElementDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),std::out_of_range);
}

TEST(ElementDrawer,TestInvalidWidth) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/element_drawer_invalid_size_width.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestDrawer>});
    improc::ElementDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),improc::file_processing_error);
}

TEST(ElementDrawer,TestInvalidHeight) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/element_drawer_invalid_size_height.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestDrawer>});
    improc::ElementDrawer drawer {};
    EXPECT_THROW(drawer.Load(factory,json_content),improc::file_processing_error);
}

TEST(ElementDrawer,TestInvalidScale) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/element_drawer_invalid_scale.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestDrawer>});
    improc::ElementDrawer drawer {};
    drawer.Load(factory,json_content);
    EXPECT_THROW(drawer.Draw(),improc::file_processing_error);
}

TEST(ElementDrawer,TestDrawWithoutTransforms) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("increment",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestDrawer>});
    improc::ElementDrawer drawer = improc::ElementDrawer(factory,json_content);
    cv::Mat test_mat  = drawer.Draw();
    cv::Mat false_mat = cv::Mat::zeros(test_mat.rows + 20,test_mat.cols,test_mat.type());
    EXPECT_EQ(test_mat.rows,10);
    EXPECT_EQ(test_mat.cols,20);
    EXPECT_TRUE(drawer.Verify(test_mat));
    EXPECT_FALSE(drawer.Verify(false_mat));
}

TEST(ElementDrawer,TestDrawWithTransforms) {
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/element_drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestDrawer>});
    improc::ElementDrawer drawer = improc::ElementDrawer(factory,json_content);
    cv::Mat test_mat  = drawer.Draw();
    cv::Mat false_mat = cv::Mat::zeros(test_mat.rows + 20,test_mat.cols,test_mat.type());
    EXPECT_EQ(test_mat.rows,40);
    EXPECT_EQ(test_mat.cols,20);
    EXPECT_TRUE(drawer.Verify(test_mat));
    EXPECT_FALSE(drawer.Verify(false_mat));
}
