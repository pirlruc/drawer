#include <improc/drawer/engine/metric_pixel_json_converter.hpp>

improc::MetricPixelJsonConverter::MetricPixelJsonConverter() {};

Json::Value improc::MetricPixelJsonConverter::Convert(const Json::Value& metric_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Converting metric json to pixel json...");
    static const std::string kPrintingResolutionKey = "printing-resolution-dpi";
    static const std::string kLayoutKey             = "layout";
    if (metric_json.isMember(kPrintingResolutionKey) == false)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: Printing resolution missing.");
        throw improc::file_processing_error();
    }
    if (metric_json.isMember(kLayoutKey) == false)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: Layout information missing.");
        throw improc::file_processing_error();
    }
    improc::MetricPixelConverter pixel_converter {improc::json::ReadElement<unsigned int>(metric_json[kPrintingResolutionKey])};

    Json::Value layout_json {std::move(metric_json[kLayoutKey])};
    if (layout_json.isArray() == true)
    {
        improc::MetricPixelJsonConverter::ParseArray(pixel_converter,layout_json);
    }
    else if (layout_json.isObject() == true)
    {
        improc::MetricPixelJsonConverter::ParseObject(pixel_converter,kLayoutKey,layout_json);
    }
    return layout_json;
}

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
        if (object_json.isMember(kMetricUnitKey) == false)
        {
            IMPROC_DRAWER_LOGGER_ERROR("ERROR_03: Metric units for {} missing.",object_name);
            throw improc::file_processing_error();
        }
        improc::MetricUnit metric = improc::json::ReadElement<std::string>(object_json[kMetricUnitKey]);
        if (object_name == kGridSpacingKey || object_name == kTopLeftKey)
        {
            cv::Point2d point  = improc::MetricPixelJsonConverter::ParsePoint(object_json);
            object_json.removeMember(kMetricUnitKey);
            object_json["x"] = pixel_converter.Metric2Pixel(point.x,metric);
            object_json["y"] = pixel_converter.Metric2Pixel(point.y,metric);
        }
        else if (object_name == kPageSizeKey || object_name == kElemSizeKey)
        {
            cv::Size2d  size   = improc::MetricPixelJsonConverter::ParseSize(object_json);
            object_json.removeMember(kMetricUnitKey);
            object_json["width"]  = pixel_converter.Metric2Pixel(size.width,metric);
            object_json["height"] = pixel_converter.Metric2Pixel(size.height,metric);
        }
    }
    else
    {
        improc::MetricPixelJsonConverter::ParseArray(pixel_converter,object_json);
    }
}

void improc::MetricPixelJsonConverter::ParseArray(const improc::MetricPixelConverter& pixel_converter, Json::Value& array_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Parsing json array...");
    //TODO: Increase performance using execution policy
    for (Json::Value::iterator item_json = array_json.begin(); item_json != array_json.end(); item_json++)
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

cv::Point2d improc::MetricPixelJsonConverter::ParsePoint(const Json::Value& json_elem)
{
    IMPROC_DRAWER_LOGGER_TRACE("Reading cv::Point json element...");
    static const std::string kXPositionKey = "x";
    static const std::string kYPositionKey = "y";
    if (json_elem.isMember(kXPositionKey) == false)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: x-position missing.");
        throw improc::file_processing_error();
    }
    if (json_elem.isMember(kYPositionKey) == false)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: y-position missing.");
        throw improc::file_processing_error();
    }
    return cv::Point2d( improc::json::ReadElement<double>(json_elem[kXPositionKey])
                      , improc::json::ReadElement<double>(json_elem[kYPositionKey]) );
}

cv::Size2d improc::MetricPixelJsonConverter::ParseSize(const Json::Value& json_elem)
{
    IMPROC_DRAWER_LOGGER_TRACE("Reading cv::Size json element...");
    static const std::string kWidthKey  = "width";
    static const std::string kHeightKey = "height";
    if (json_elem.isMember(kWidthKey) == false)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: Width missing.");
        throw improc::file_processing_error();
    }
    if (json_elem.isMember(kHeightKey) == false)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: Height missing.");
        throw improc::file_processing_error();
    }
    return cv::Size2d( improc::json::ReadElement<double>(json_elem[kWidthKey])
                     , improc::json::ReadElement<double>(json_elem[kHeightKey]) );
}
