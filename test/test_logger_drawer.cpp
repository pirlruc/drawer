#include <gtest/gtest.h>

#include <improc/drawer/logger_drawer.hpp>

TEST(LoggerDrawer,TestLoggerDrawer) {
    improc::DrawerLogger::get("drawer_logger");
    EXPECT_STREQ(improc::DrawerLogger::get()->data()->name().c_str(),"drawer_logger");
    IMPROC_DRAWER_LOGGER_TRACE("Test {} {}",1,2);
    IMPROC_DRAWER_LOGGER_DEBUG("Test {} {}",2,3);
    IMPROC_DRAWER_LOGGER_INFO ("Test {} {}",3,4);
    IMPROC_DRAWER_LOGGER_ERROR("Test {} {}",4,5);
    IMPROC_DRAWER_LOGGER_WARN ("Test {} {}",5,6);
    IMPROC_DRAWER_LOGGER_CRITICAL("Test {} {}",6,7);
}