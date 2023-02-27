#include <improc/drawer/engine/metric_pixel_converter.hpp>

/**
 * @brief Construct a new improc::MetricUnit object
 */
improc::MetricUnit::MetricUnit() : value_(improc::MetricUnit::kMilimiter) {};

/**
 * @brief Construct a new improc::MetricUnit object
 * 
 * @param metric_unit_str - metric unit description as string
 */
improc::MetricUnit::MetricUnit(const std::string& metric_unit_str)
{
    IMPROC_DRAWER_LOGGER_TRACE("Obtaining metric unit from string {}...",metric_unit_str);
    static const std::unordered_map<std::string,MetricUnit::Value> kToElemType = { {"m" ,MetricUnit::Value::kMeter     }
                                                                                 , {"dm",MetricUnit::Value::kDecimeter }
                                                                                 , {"cm",MetricUnit::Value::kCentimeter}
                                                                                 , {"mm",MetricUnit::Value::kMilimiter }
                                                                                 };
    this->value_ = kToElemType.at(improc::String::ToLower(std::move(metric_unit_str)));
}

/**
 * @brief Construct a new improc::MetricPixelConverter object
 */
improc::MetricPixelConverter::MetricPixelConverter() : metric_to_pixel_factor_(600.0 / MetricPixelConverter::kInchToMilimeter) {};

/**
 * @brief Construct a new improc::MetricPixelConverter object
 * 
 * @param printing_resolution_dpi - printing resolution in dpis.
 */
improc::MetricPixelConverter::MetricPixelConverter(unsigned int printing_resolution_dpi)
{
    this->set_printing_resolution(std::move(printing_resolution_dpi));
}

/**
 * @brief Set printing resolution
 * 
 * @param printing_resolution_dpi - printing resolution in dpis.S
 */
improc::MetricPixelConverter& improc::MetricPixelConverter::set_printing_resolution(unsigned int printing_resolution_dpi)
{
    IMPROC_DRAWER_LOGGER_TRACE("Setting printing resolution...");
    if (printing_resolution_dpi == 0)
    {
        std::string error_message = fmt::format("Printing resolution should be positive and not {}",printing_resolution_dpi);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::value_error(std::move(error_message));
    }
    this->metric_to_pixel_factor_ = static_cast<double>(printing_resolution_dpi) / MetricPixelConverter::kInchToMilimeter;
    return (*this);
}
