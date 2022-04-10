#include <improc/drawer/metric_pixel_converter.hpp>

improc::MetricUnit::MetricUnit() : value_(improc::MetricUnit::kMilimiter) {};

improc::MetricUnit::MetricUnit(const std::string& metric_unit_str)
{
    IMPROC_DRAWER_LOGGER_TRACE("Obtaining metric unit from string {}...",metric_unit_str);
    static const std::unordered_map<std::string,MetricUnit::Value> kToElemType = { {"m" ,MetricUnit::Value::kMeter     }
                                                                                 , {"dm",MetricUnit::Value::kDecimeter }
                                                                                 , {"cm",MetricUnit::Value::kCentimeter}
                                                                                 , {"mm",MetricUnit::Value::kMilimiter }
                                                                                 };
    this->value_ = kToElemType.at(improc::String::ToLower(metric_unit_str));
}

#include <iostream>
double improc::MetricUnit::GetConversionFactor(const improc::MetricUnit& to_metric_unit) const
{
    IMPROC_DRAWER_LOGGER_TRACE("Obtaining conversion factor from {} to {}...",this->ToString(),to_metric_unit.ToString());
    return std::pow(10,to_metric_unit.operator improc::MetricUnit::Value() - this->value_);
}

improc::MetricPixelConverter::MetricPixelConverter() : metric_to_pixel_factor_(600.0 / MetricPixelConverter::kInchToMilimeter) {};

improc::MetricPixelConverter::MetricPixelConverter(unsigned int printing_resolution_dpi)
{
    this->set_printing_resolution(printing_resolution_dpi);
}

improc::MetricPixelConverter& improc::MetricPixelConverter::set_printing_resolution(unsigned int printing_resolution_dpi)
{
    IMPROC_DRAWER_LOGGER_TRACE("Setting printing resolution...");
    this->metric_to_pixel_factor_ = static_cast<double>(printing_resolution_dpi) / MetricPixelConverter::kInchToMilimeter;
    return (*this);
}

unsigned int improc::MetricPixelConverter::Metric2Pixel(double metric, improc::MetricUnit from_metric_unit) const
{
    IMPROC_DRAWER_LOGGER_TRACE("Converting metric to pixel units...");
    return std::round(metric * (this->metric_to_pixel_factor_ * from_metric_unit.GetConversionFactor(improc::MetricPixelConverter::kMilimeter)));
}

double improc::MetricPixelConverter::Pixel2Metric(unsigned int pixel, improc::MetricUnit to_metric_unit) const
{
    IMPROC_DRAWER_LOGGER_TRACE("Converting pixel to metric units...");
    return static_cast<double>(pixel) / (this->metric_to_pixel_factor_ * to_metric_unit.GetConversionFactor(improc::MetricPixelConverter::kMilimeter));
}
