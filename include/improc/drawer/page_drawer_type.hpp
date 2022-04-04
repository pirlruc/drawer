#ifndef IMPROC_DRAWER_PAGE_DRAWER_TYPE_HPP
#define IMPROC_DRAWER_PAGE_DRAWER_TYPE_HPP

#include <improc/improc_defs.hpp>
#include <improc/drawer/logger_drawer.hpp>
#include <improc/infrastructure/string.hpp>

#define IMPROC_ENUM_KEY_TYPE unsigned int

#include <opencv2/core.hpp>

namespace improc 
{
    class IMPROC_EXPORTS PageDrawerType
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
            PageDrawerType(const std::string& page_drawer_type_str);
            constexpr                   PageDrawerType(Value page_drawer_type_value): value_(page_drawer_type_value) {}
            constexpr operator          Value()     const {return this->value_;}

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