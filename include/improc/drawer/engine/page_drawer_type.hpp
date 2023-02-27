#ifndef IMPROC_DRAWER_PAGE_DRAWER_TYPE_HPP
#define IMPROC_DRAWER_PAGE_DRAWER_TYPE_HPP

#include <improc/improc_defs.hpp>
#include <improc/drawer/logger_drawer.hpp>
#include <improc/infrastructure/string.hpp>

#include <opencv2/core.hpp>

namespace improc 
{
    /**
     * @brief Page drawer type methods and utilities
     */
    class IMPROC_API PageDrawerType final
    {
        public:
            enum Value : IMPROC_ENUM_KEY_TYPE
            {
                    kPageDrawer = 0
                ,   kGridDrawer = 1
            };

        private:
            Value                       value_;

        public:
            PageDrawerType();                              
            explicit PageDrawerType(const std::string& page_drawer_type_str);

            /**
             * @brief Construct a new improc::PageDrawerType object
             * 
             * @param page_drawer_type_value - page drawer type value
             */
            constexpr explicit          PageDrawerType(Value page_drawer_type_value): value_(std::move(page_drawer_type_value)) {}

            /**
             * @brief Obtain page drawer type value
             */
            constexpr operator          Value()     const {return this->value_;}

            /**
             * @brief Obtain page drawer type string description
             */
            constexpr std::string_view  ToString()  const
            {
                switch (this->value_)
                {
                    case PageDrawerType::Value::kPageDrawer : return "Page Drawer";    break;
                    case PageDrawerType::Value::kGridDrawer : return "Grid Drawer";   break;
                }
            }
    };
}

#endif