#include <improc/drawer/engine/metric_pixel_json_converter.hpp>

/**
 * @brief Construct a new improc::MetricPixelJsonConverter object
 */
improc::MetricPixelJsonConverter::MetricPixelJsonConverter() {};

/**
 * @brief Convert json configuration in metric units to json configuration in pixel units
 * 
 * @param metric_json - json configuration in metric units
 * @return Json::Value - json configuration in pixel units
 */
Json::Value improc::MetricPixelJsonConverter::Convert(const Json::Value& metric_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Converting metric json to pixel json...");
    static const std::string kPrintingResolutionKey = "printing-resolution-dpi";
    static const std::string kLayoutKey             = "layout";
    if (metric_json.isMember(kPrintingResolutionKey) == false)
    {
        std::string error_message = fmt::format("Key {} is missing from metric pixel json converter json",kPrintingResolutionKey);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::json_error(std::move(error_message));
    }
    if (metric_json.isMember(kLayoutKey) == false)
    {
        std::string error_message = fmt::format("Key {} is missing from metric pixel json converter json",kLayoutKey);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: " + error_message);
        throw improc::json_error(std::move(error_message));
    }
    improc::MetricPixelConverter pixel_converter {improc::json::ReadElement<unsigned int>(metric_json[kPrintingResolutionKey])};

    Json::Value layout_json {std::move(metric_json[kLayoutKey])};
    if (layout_json.isArray() == true)
    {
        improc::MetricPixelJsonConverter::ParseArray(std::move(pixel_converter),layout_json);
    }
    else if (layout_json.isObject() == true)
    {
        improc::MetricPixelJsonConverter::ParseObject(std::move(pixel_converter),kLayoutKey,layout_json);
    }
    return layout_json;
}

/**
 * @brief Parse json drawer factory object
 * 
 * @param pixel_converter - pixel unit converter
 * @param object_name - drawer factory object name
 * @param object_json - configuration json in metric units
 * @return object_json - configuration json in pixel units
 */
void improc::MetricPixelJsonConverter::ParseObject(const improc::MetricPixelConverter& pixel_converter, const std::string& object_name, Json::Value& object_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Parsing json object...");
    static const std::string kElemSizeKey           = "drawer-size"; // Element drawer
    static const std::string kGridSpacingKey        = "spacing";     // Grid drawer
    static const std::string kTopLeftKey            = "top-left";    // Layout + Page element drawers
    static const std::string kPageSizeKey           = "page-size";   // Page drawer
    static const std::string kMetricUnitKey         = "metric-unit";
    if (object_name == kElemSizeKey || object_name == kGridSpacingKey || object_name == kTopLeftKey || object_name == kPageSizeKey)
    {
        if (object_name == kGridSpacingKey || object_name == kTopLeftKey)
        {
            cv::Point point  = improc::MetricPixelJsonConverter::MetricPoint2PixelPoint(object_json,std::move(pixel_converter));
            object_json.removeMember(kMetricUnitKey);
            object_json["x"] = point.x;
            object_json["y"] = point.y;
        }
        else if (object_name == kPageSizeKey || object_name == kElemSizeKey)
        {
            cv::Size size         = improc::MetricPixelJsonConverter::MetricSize2PixelSize(object_json,std::move(pixel_converter));
            object_json.removeMember(kMetricUnitKey);
            object_json["width"]  = size.width;
            object_json["height"] = size.height;
        }
    }
    else
    {
        improc::MetricPixelJsonConverter::ParseArray(std::move(pixel_converter),object_json);
    }
}

/**
 * @brief Convert metric size to pixel size
 * 
 * @param metric_size_json - configuration json for metric size
 * @param pixel_converter - pixel unit converter
 * @return cv::Size - size in pixel units
 */
cv::Size improc::MetricPixelJsonConverter::MetricSize2PixelSize(const Json::Value& metric_size_json, const improc::MetricPixelConverter& pixel_converter)
{
    IMPROC_DRAWER_LOGGER_TRACE("Parsing metric size json object...");
    static const std::string kMetricUnitKey         = "metric-unit";
    if (metric_size_json.isMember(kMetricUnitKey) == false)
    {
        std::string error_message = fmt::format("Key {} is missing from metric to pixel size json",kMetricUnitKey);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::json_error(std::move(error_message));
    }
    improc::MetricUnit metric = improc::MetricUnit(improc::json::ReadElement<std::string>(metric_size_json[kMetricUnitKey]));
    cv::Size2d metric_size    = improc::json::ReadElement<cv::Size2d>(std::move(metric_size_json));
    return cv::Size(pixel_converter.Metric2Pixel(metric_size.width,metric),pixel_converter.Metric2Pixel(metric_size.height,metric));
}

/**
 * @brief Convert metric point to pixel point
 * 
 * @param metric_point_json - configuration json for metric point
 * @param pixel_converter - pixel unit converter
 * @return cv::Point - point in pixel units
 */
cv::Point improc::MetricPixelJsonConverter::MetricPoint2PixelPoint(const Json::Value& metric_point_json, const improc::MetricPixelConverter& pixel_converter)
{
    IMPROC_DRAWER_LOGGER_TRACE("Parsing metric point json object...");
    static const std::string kMetricUnitKey         = "metric-unit";
    if (metric_point_json.isMember(kMetricUnitKey) == false)
    {
        std::string error_message = fmt::format("Key {} is missing from metric to pixel point json",kMetricUnitKey);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::json_error(std::move(error_message));
    }
    improc::MetricUnit metric = improc::MetricUnit(improc::json::ReadElement<std::string>(metric_point_json[kMetricUnitKey]));
    cv::Point2d metric_point  = improc::json::ReadElement<cv::Point2d>(std::move(metric_point_json));
    return cv::Point(pixel_converter.Metric2Pixel(metric_point.x,metric),pixel_converter.Metric2Pixel(metric_point.y,metric));
}

/**
 * @brief Parse json array sructure
 * 
 * @param pixel_converter - pixel unit converter
 * @param array_json - configuration json array
 * @return array_json - configuration json array in pixel units
 */
void improc::MetricPixelJsonConverter::ParseArray(const improc::MetricPixelConverter& pixel_converter, Json::Value& array_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Parsing json array...");
    for (Json::Value::iterator item_json = array_json.begin(); item_json != array_json.end(); ++item_json)
    {
        if(item_json->isArray() == true)
        {
            improc::MetricPixelJsonConverter::ParseArray(pixel_converter,*item_json);
        }
        else if (item_json->isObject() == true)
        {
            improc::MetricPixelJsonConverter::ParseObject(pixel_converter,item_json.name(),*item_json);
        }
    }
}
