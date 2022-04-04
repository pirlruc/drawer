#include <gtest/gtest.h>

#include <improc/drawer/page_drawer_type.hpp>

TEST(PageDrawerType,TestEmptyConstructor) {
    improc::PageDrawerType drawer_type {};
    EXPECT_EQ(drawer_type,improc::PageDrawerType::Value::kPageDrawer);
}

TEST(PageDrawerType,TestConstructorFromValue) {
    improc::PageDrawerType drawer_type {improc::PageDrawerType::Value::kGridDrawer};
    EXPECT_EQ(drawer_type,improc::PageDrawerType::Value::kGridDrawer);
}

TEST(PageDrawerType,TestConstructorFromLowerString) {
    improc::PageDrawerType drawer_type_page  {"page-drawer"};
    improc::PageDrawerType drawer_type_grid  {"grid-drawer"};
    EXPECT_EQ(drawer_type_page,improc::PageDrawerType::Value::kPageDrawer);
    EXPECT_EQ(drawer_type_grid,improc::PageDrawerType::Value::kGridDrawer);
}

TEST(PageDrawerType,TestConstructorFromUpperString) {
    improc::PageDrawerType drawer_type_page  {"PAGE-DRAWER"};
    improc::PageDrawerType drawer_type_grid  {"GRID-DRAWER"};
    EXPECT_EQ(drawer_type_page,improc::PageDrawerType::Value::kPageDrawer);
    EXPECT_EQ(drawer_type_grid,improc::PageDrawerType::Value::kGridDrawer);
}

TEST(PageDrawerType,TestInvalidDrawerTypeConstructor) {
    EXPECT_THROW(improc::PageDrawerType drawer_type {"invalid"},std::out_of_range);
}

TEST(PageDrawerType,TestConstructorFromClass) {
    EXPECT_EQ(improc::PageDrawerType::kPageDrawer,improc::PageDrawerType::Value::kPageDrawer);
    EXPECT_EQ(improc::PageDrawerType::kGridDrawer,improc::PageDrawerType::Value::kGridDrawer);
}

TEST(PageDrawerType,TestToString) {
    improc::PageDrawerType drawer_type_page  {"page-drawer"};
    improc::PageDrawerType drawer_type_grid  {"grid-drawer"};
    EXPECT_EQ(drawer_type_page.ToString(),"Page Drawer");
    EXPECT_EQ(drawer_type_grid.ToString(),"Grid Drawer");
}
