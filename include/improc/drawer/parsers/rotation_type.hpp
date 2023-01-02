#ifndef IMPROC_DRAWER_ROTATION_TYPE_HPP
#define IMPROC_DRAWER_ROTATION_TYPE_HPP

#include <improc/improc_defs.hpp>
#include <improc/drawer/logger_drawer.hpp>
#include <improc/infrastructure/string.hpp>

#include <opencv2/core.hpp>

// TODO: Move to core_cv
namespace improc 
{
    class IMPROC_API RotationType
    {
        public:
            enum Value : IMPROC_ENUM_KEY_TYPE
            {
                    k0Deg   = 0
                ,   k90Deg  = 1
                ,   k180Deg = 2
                ,   k270Deg = 3
            };

        private:
            Value                       value_;

        public:
            RotationType();                              
            RotationType(const std::string& rotation_type_str);
            constexpr                   RotationType(Value rotation_type_value): value_(rotation_type_value) {}
            constexpr operator          Value()     const {return this->value_;}

            constexpr std::string_view  ToString()  const
            {
                switch (this->value_)
                {
                    case RotationType::Value::k0Deg  : return "0 Degrees";    break;
                    case RotationType::Value::k90Deg : return "90 Degrees";   break;
                    case RotationType::Value::k180Deg: return "180 Degrees";  break;
                    case RotationType::Value::k270Deg: return "270 Degrees";  break;
                }
            }

            cv::Mat                     Apply       (const cv::Mat& image)          const;
            cv::Mat                     ApplyInverse(const cv::Mat& rotated_image)  const;
    };
}

#endif