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
            MetricUnit(const std::string& metric_unit_str);
            constexpr                   MetricUnit(Value metric_unit_value): value_(metric_unit_value) {}
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

            double                      GetConversionFactor(const MetricUnit& to_metric_unit) const;
    };

    class IMPROC_API MetricPixelConverter
    {
        private:
            static constexpr double     kInchToMilimeter = 25.4;
            static constexpr MetricUnit kMilimeter       = improc::MetricUnit::kMilimiter;
            double                      metric_to_pixel_factor_;

        public:
            MetricPixelConverter();
            MetricPixelConverter(unsigned int printing_resolution_dpi);

            MetricPixelConverter&       set_printing_resolution(unsigned int printing_resolution_dpi);

            unsigned int                Metric2Pixel(double       metric, improc::MetricUnit from_metric_unit = improc::MetricUnit::kMilimiter) const;
            double                      Pixel2Metric(unsigned int pixel , improc::MetricUnit to_metric_unit   = improc::MetricUnit::kMilimiter) const;
    };
}

#endif