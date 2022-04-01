#include <improc/drawer/page_element_drawer.hpp>

improc::PageElementDrawer::PageElementDrawer()  : improc::ElementDrawer()
                                                , top_left_(cv::Point()) {};

improc::PageElementDrawer::PageElementDrawer(const improc::DrawerFactory& factory, const Json::Value& page_element_drawer_json, const cv::Size& page_size) : improc::PageElementDrawer()
{
    this->Load(factory,page_element_drawer_json,page_size);
}

improc::PageElementDrawer& improc::PageElementDrawer::Load(const improc::DrawerFactory& factory, const Json::Value& page_element_drawer_json, const cv::Size& page_size) 
{
    IMPROC_DRAWER_LOGGER_TRACE("Creating page element drawer...");
    static const std::string kTopLeftKey = "top-left";
    if (page_element_drawer_json.isMember(kTopLeftKey) == false)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: Top left position missing.");
        throw improc::file_processing_error();
    }
    this->improc::ElementDrawer::Load(factory,page_element_drawer_json);
    this->top_left_ = improc::PageElementDrawer::ParsePoint(page_element_drawer_json[kTopLeftKey],page_size);
    return (*this);
}

cv::Point improc::PageElementDrawer::ParsePoint(const Json::Value& point_json, const cv::Size& page_size)
{
    IMPROC_DRAWER_LOGGER_TRACE("Parsing point...");
    cv::Point point = improc::json::ReadElement<cv::Point>(point_json);
    if (improc::PageElementDrawer::IsPixelPositionValid(point.x,page_size.width) == false)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: Invalid x-position. It should be between 0 and {}.",page_size.width);
        throw improc::file_processing_error();
    }
    if (improc::PageElementDrawer::IsPixelPositionValid(point.y,page_size.height) == false)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: Invalid y-position. It should be between 0 and {}.",page_size.height);
        throw improc::file_processing_error();
    }
    return point;
}

bool improc::PageElementDrawer::IsPixelPositionValid(int pixel_position, int max_pixel_size)
{
    IMPROC_DRAWER_LOGGER_TRACE("Validating pixel position...");
    bool result = true;
    if (pixel_position < 0 || pixel_position >= max_pixel_size)
    {
        result = false;
    }
    return result;
}

void improc::PageElementDrawer::Draw(cv::Mat& page_image) const
{
    IMPROC_DRAWER_LOGGER_TRACE("Drawing page element...");
    this->ElementDrawer::Draw().copyTo(page_image(cv::Rect(this->top_left_,this->ElementDrawer::Draw().size())));
}
