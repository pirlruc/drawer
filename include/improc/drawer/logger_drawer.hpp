#ifndef IMPROC_DRAWER_LOGGER_DRAWER_HPP
#define IMPROC_DRAWER_LOGGER_DRAWER_HPP

#include <improc/improc_defs.hpp>
#include <improc/infrastructure/logging/logger_singleton.hpp>

namespace improc 
{
    class IMPROC_API DrawerLogger final: public LoggerSingleton<DrawerLogger>
    {
        friend std::shared_ptr<DrawerLogger> LoggerSingleton::get(const std::string& logger_name);
        private:
            explicit DrawerLogger(std::shared_ptr<spdlog::logger>&&      logger) : LoggerSingleton(std::move(logger)) {}

        public:
            DrawerLogger(DrawerLogger&  that) = delete;
            DrawerLogger(DrawerLogger&& that) = delete;
            void operator=(const DrawerLogger&  that) = delete;
            void operator=(const DrawerLogger&& that) = delete;
    };
}

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_TRACE
#    define IMPROC_DRAWER_LOGGER_TRACE(...) IMPROC_LOGGER_TRACE(improc::DrawerLogger::get(), __VA_ARGS__)
#else
#    define IMPROC_DRAWER_LOGGER_TRACE(...) (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_DEBUG
#    define IMPROC_DRAWER_LOGGER_DEBUG(...) IMPROC_LOGGER_DEBUG(improc::DrawerLogger::get(), __VA_ARGS__)
#else
#    define IMPROC_DRAWER_LOGGER_DEBUG(...) (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_INFO
#    define IMPROC_DRAWER_LOGGER_INFO(...) IMPROC_LOGGER_INFO(improc::DrawerLogger::get(), __VA_ARGS__)
#else
#    define IMPROC_DRAWER_LOGGER_INFO(...) (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_WARN
#    define IMPROC_DRAWER_LOGGER_WARN(...) IMPROC_LOGGER_WARN(improc::DrawerLogger::get(), __VA_ARGS__)
#else
#    define IMPROC_DRAWER_LOGGER_WARN(...) (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_ERROR
#    define IMPROC_DRAWER_LOGGER_ERROR(...) IMPROC_LOGGER_ERROR(improc::DrawerLogger::get(), __VA_ARGS__)
#else
#    define IMPROC_DRAWER_LOGGER_ERROR(...) (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_CRITICAL
#    define IMPROC_DRAWER_LOGGER_CRITICAL(...) IMPROC_LOGGER_CRITICAL(improc::DrawerLogger::get(), __VA_ARGS__)
#else
#    define IMPROC_DRAWER_LOGGER_CRITICAL(...) (void)0
#endif

#endif