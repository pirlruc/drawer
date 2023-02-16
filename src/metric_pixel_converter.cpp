#include <improc/drawer/engine/metric_pixel_converter.hpp>

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

improc::MetricPixelConverter::MetricPixelConverter() : metric_to_pixel_factor_(600.0 / MetricPixelConverter::kInchToMilimeter) {};

improc::MetricPixelConverter::MetricPixelConverter(unsigned int printing_resolution_dpi)
{
    this->set_printing_resolution(printing_resolution_dpi);
}

improc::MetricPixelConverter& improc::MetricPixelConverter::set_printing_resolution(unsigned int printing_resolution_dpi)
{
    IMPROC_DRAWER_LOGGER_TRACE("Setting printing resolution...");
    if (printing_resolution_dpi == 0)
    {
        IMPROC_DRAWER_LOGGER_ERROR("Printing resolution should be positive");
        throw improc::printing_resolution_not_positive();
    }

    this->metric_to_pixel_factor_ = static_cast<double>(printing_resolution_dpi) / MetricPixelConverter::kInchToMilimeter;
    return (*this);
}
