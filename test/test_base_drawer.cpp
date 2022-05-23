#include <gtest/gtest.h>

#include <improc_drawer_test_config.hpp>

#include <improc/drawer/base_drawer.hpp>
#include <improc/infrastructure/filesystem/file.hpp>

class TestDrawer : public improc::BaseDrawer
{
    public:
        TestDrawer() {};
        TestDrawer(const Json::Value& drawer_json)
        {
            this->Load(drawer_json);
        }

        TestDrawer& Load(const Json::Value& drawer_json)
        {
            return (*this);
        }

        cv::Mat     Draw(const std::optional<std::string>& message = std::optional<std::string>())  const
        {
            return cv::Mat::ones(10,20,CV_8UC1);
        }
};

TEST(BaseDrawer,TestConstructor) {
    EXPECT_NO_THROW(TestDrawer());
}

TEST(BaseDrawer,TestConstructorWithLoad) {
    Json::Value drawer_json {};
    EXPECT_NO_THROW(TestDrawer {drawer_json});
}

TEST(BaseDrawer,TestLoading) {
    TestDrawer test {};
    Json::Value drawer_json {};
    EXPECT_NO_THROW(test.Load(drawer_json));
}

TEST(BaseDrawer,TestDraw) {
    TestDrawer test {};
    Json::Value drawer_json {};
    cv::Mat test_mat = test.Draw();
    EXPECT_EQ(test_mat.rows,10);
    EXPECT_EQ(test_mat.cols,20);
}

TEST(DrawerFactory,TestConstructor) {
    EXPECT_EQ(improc::DrawerFactory().GetRegisteredIds().size(),0);
}

TEST(DrawerFactory,TestRegister) {
    improc::DrawerFactory factory {};
    Json::Value drawer_json {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestDrawer>});
    std::shared_ptr<improc::BaseDrawer> drawer = factory.Create("test_drawer",drawer_json);
    cv::Mat test_mat = drawer->Draw();
    EXPECT_EQ(test_mat.rows,10);
    EXPECT_EQ(test_mat.cols,20);
}

TEST(BaseDrawer,TestNoDrawerType)
{
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/drawer_no_type.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestDrawer>});
    EXPECT_THROW(improc::BaseDrawer::Create(factory,json_content),improc::file_processing_error);
}

TEST(BaseDrawer,TestNoDrawerInFactory)
{
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("test_drawer",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestDrawer>});
    EXPECT_THROW(improc::BaseDrawer::Create(factory,json_content),improc::not_found_in_factory);
}

TEST(BaseDrawer,TestDrawerInFactory)
{
    std::string json_filepath = std::string(IMPROC_DRAWER_TEST_FOLDER) + "/test/data/drawer_config.json";
    Json::Value json_content  = improc::JsonFile::Read(json_filepath);
    improc::DrawerFactory factory {};
    factory.Register("increment",std::function<std::shared_ptr<improc::BaseDrawer>(const Json::Value&)> {&improc::CreateDrawer<TestDrawer>});
    EXPECT_NO_THROW(improc::BaseDrawer::Create(factory,json_content));
}
