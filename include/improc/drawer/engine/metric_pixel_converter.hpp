#ifndef IMPROC_DRAWER_METRIC_PIXEL_CONVERSION_HPP
#define IMPROC_DRAWER_METRIC_PIXEL_CONVERSION_HPP

#include <improc/improc_defs.hpp>
#include <improc/drawer/logger_drawer.hpp>
#include <improc/infrastructure/string.hpp>

#include <json/json.h>

namespace improc 
{
    class IMPROC_API MetricUnit
    {
        public:
            enum Value : int
            {
                    kMeter      = 0
                ,   kDecimeter  = 1
                ,   kCentimeter = 2
                ,   kMilimiter  = 3
            };

        private:
            Value                       value_;

        public:
            MetricUnit();                              
            explicit MetricUnit(const std::string& metric_unit_str);
            constexpr explicit          MetricUnit(Value metric_unit_value): value_(metric_unit_value) {}
            constexpr operator          Value()     const {return this->value_;}

            constexpr std::string_view  ToString()  const
            {
                switch (this->value_)
                {
                    case MetricUnit::Value::kMeter     : return "Meter";       break;
                    case MetricUnit::Value::kDecimeter : return "Decimeter";   break;
                    case MetricUnit::Value::kCentimeter: return "Centimeter";  break;
                    case MetricUnit::Value::kMilimiter : return "Milimeter";   break;
                };
            }

            template <typename MetricUnitType>
            double                      GetConversionFactor(const MetricUnitType& to_metric_unit) const
            {
                if constexpr (std::is_same_v<MetricUnitType,improc::MetricUnit>)
                {
                    IMPROC_DRAWER_LOGGER_TRACE("Obtaining conversion factor from {} to {}...",this->ToString(),to_metric_unit.ToString());
                    return std::pow(10,to_metric_unit.operator improc::MetricUnit::Value() - this->value_);
                }
                else if constexpr (std::is_same_v<MetricUnitType,improc::MetricUnit::Value>)
                {
                    IMPROC_DRAWER_LOGGER_TRACE("Obtaining conversion factor from {} to {}...",this->ToString(),improc::MetricUnit(to_metric_unit).ToString());
                    return std::pow(10,to_metric_unit - this->value_);
                }
                else
                {
                    static_assert(improc::dependent_false_v<MetricUnitType>,"Conversion factor not defined for metric unit type");
                }
            }
    };

    class IMPROC_API MetricPixelConverter
    {
        private:
            static constexpr double             kInchToMilimeter = 25.4;
            static constexpr MetricUnit::Value  kMilimeter       = improc::MetricUnit::kMilimiter;
            double                              metric_to_pixel_factor_;

        public:
            MetricPixelConverter();
            explicit MetricPixelConverter(unsigned int printing_resolution_dpi);

            MetricPixelConverter&       set_printing_resolution(unsigned int printing_resolution_dpi);

            template <typename MetricUnitType = improc::MetricUnit::Value>
            unsigned int                Metric2Pixel(double       metric, const MetricUnitType& from_metric_unit = improc::MetricUnit::kMilimiter) const
            {
                IMPROC_DRAWER_LOGGER_TRACE("Converting metric to pixel units...");
                if constexpr (std::is_same_v<MetricUnitType,improc::MetricUnit>)
                {
                    return std::round(metric * (this->metric_to_pixel_factor_ * from_metric_unit.GetConversionFactor(improc::MetricPixelConverter::kMilimeter)));
                }
                else if constexpr (std::is_same_v<MetricUnitType,improc::MetricUnit::Value>)
                {
                    return std::round(metric * (this->metric_to_pixel_factor_ * improc::MetricUnit(from_metric_unit).GetConversionFactor(improc::MetricPixelConverter::kMilimeter)));
                }
                else
                {
                    static_assert(improc::dependent_false_v<MetricUnitType>,"Conversion not defined for metric unit type");
                }
            }

            template <typename MetricUnitType = improc::MetricUnit::Value>
            double                      Pixel2Metric(unsigned int pixel , const MetricUnitType& to_metric_unit   = improc::MetricUnit::kMilimiter) const
            {
                IMPROC_DRAWER_LOGGER_TRACE("Converting pixel to metric units...");
                if constexpr (std::is_same_v<MetricUnitType,improc::MetricUnit>)
                {
                    return static_cast<double>(pixel) / (this->metric_to_pixel_factor_ * to_metric_unit.GetConversionFactor(improc::MetricPixelConverter::kMilimeter));
                }
                else if constexpr (std::is_same_v<MetricUnitType,improc::MetricUnit::Value>)
                {
                    return static_cast<double>(pixel) / (this->metric_to_pixel_factor_ * improc::MetricUnit(to_metric_unit).GetConversionFactor(improc::MetricPixelConverter::kMilimeter));
                }
                else
                {
                    static_assert(improc::dependent_false_v<MetricUnitType>,"Conversion not defined for metric unit type");
                }
            }
    };
}

#endif