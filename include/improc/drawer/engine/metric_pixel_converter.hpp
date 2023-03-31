#ifndef IMPROC_DRAWER_METRIC_PIXEL_CONVERSION_HPP
#define IMPROC_DRAWER_METRIC_PIXEL_CONVERSION_HPP

#include <improc/improc_defs.hpp>
#include <improc/drawer/logger_drawer.hpp>
#include <improc/infrastructure/string.hpp>

#include <json/json.h>

namespace improc 
{
    /**
     * @brief Metric unit methods and utilities
     */
    class IMPROC_API MetricUnit final
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

            /**
             * @brief Construct a new improc::MetricUnit object
             * 
             * @param metric_unit_value - metric unit value
             */
            constexpr explicit          MetricUnit(Value metric_unit_value): value_(std::move(metric_unit_value)) {}

            /**
             * @brief Obtain metric unit value
             */
            constexpr operator          Value()     const {return this->value_;}

            /**
             * @brief Obtain metric unit string description
             */
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

            /**
             * @brief Obtain conversion factor
             * 
             * @tparam MetricUnitType - metric unit data type: improc::MetricUnit or improc::MetricUnit::Value
             * @param to_metric_unit - target metric unit
             * @return double - conversion factor between metric units
             */
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
                    return std::pow(10,std::move(to_metric_unit) - this->value_);
                }
                else
                {
                    static_assert(improc::dependent_false_v<MetricUnitType>,"Conversion factor not defined for metric unit type");
                }
            }
    };

    /**
     * @brief Metric pixel conversion methods and utilities
     */
    class IMPROC_API MetricPixelConverter final
    {
        private:
            static constexpr double             kInchToMilimeter = 25.4;
            static constexpr MetricUnit::Value  kMilimeter       = improc::MetricUnit::kMilimiter;
            double                              metric_to_pixel_factor_;

        public:
            MetricPixelConverter();
            explicit MetricPixelConverter(unsigned int printing_resolution_dpi);

            MetricPixelConverter&       set_printing_resolution(unsigned int printing_resolution_dpi);

            /**
             * @brief Convert metric unit to pixel
             * 
             * @tparam MetricUnitType - metric unit data type: improc::MetricUnit or improc::MetricUnit::Value
             * @param metric - measure in metric units
             * @param from_metric_unit - source metric unit
             * @return int - measure in pixels
             */
            template <typename MetricUnitType = improc::MetricUnit::Value>
            int                         Metric2Pixel(double       metric, const MetricUnitType& from_metric_unit = improc::MetricUnit::kMilimiter) const
            {
                IMPROC_DRAWER_LOGGER_TRACE("Converting metric to pixel units...");
                if constexpr (std::is_same_v<MetricUnitType,improc::MetricUnit>)
                {
                    return std::round(std::move(metric) * (this->metric_to_pixel_factor_ * from_metric_unit.GetConversionFactor(improc::MetricPixelConverter::kMilimeter)));
                }
                else if constexpr (std::is_same_v<MetricUnitType,improc::MetricUnit::Value>)
                {
                    return std::round(std::move(metric) * (this->metric_to_pixel_factor_ * improc::MetricUnit(std::move(from_metric_unit)).GetConversionFactor(improc::MetricPixelConverter::kMilimeter)));
                }
                else
                {
                    static_assert(improc::dependent_false_v<MetricUnitType>,"Conversion not defined for metric unit type");
                }
            }

            /**
             * @brief Convert pixel unit to metric
             * 
             * @tparam MetricUnitType - metric unit data type: improc::MetricUnit or improc::MetricUnit::Value
             * @param pixel - measure in pixels
             * @param to_metric_unit - target metric unit
             * @return double - measure in metric units
             */
            template <typename MetricUnitType = improc::MetricUnit::Value>
            double                      Pixel2Metric(int pixel , const MetricUnitType& to_metric_unit   = improc::MetricUnit::kMilimiter) const
            {
                IMPROC_DRAWER_LOGGER_TRACE("Converting pixel to metric units...");
                if constexpr (std::is_same_v<MetricUnitType,improc::MetricUnit>)
                {
                    return static_cast<double>(std::move(pixel)) / (this->metric_to_pixel_factor_ * to_metric_unit.GetConversionFactor(improc::MetricPixelConverter::kMilimeter));
                }
                else if constexpr (std::is_same_v<MetricUnitType,improc::MetricUnit::Value>)
                {
                    return static_cast<double>(std::move(pixel)) / (this->metric_to_pixel_factor_ * improc::MetricUnit(std::move(to_metric_unit)).GetConversionFactor(improc::MetricPixelConverter::kMilimeter));
                }
                else
                {
                    static_assert(improc::dependent_false_v<MetricUnitType>,"Conversion not defined for metric unit type");
                }
            }
    };
}

#endif