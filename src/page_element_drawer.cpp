#include <improc/drawer/engine/page_element_drawer.hpp>

improc::PageElementDrawer::PageElementDrawer()  : improc::ElementDrawer()
                                                , top_left_(cv::Point())
                                                , element_box_(cv::Rect()) 
                                                , static_(false) 
                                                , content_(std::optional<std::string>()) {};

improc::PageElementDrawer::PageElementDrawer(const improc::DrawerFactory& factory, const Json::Value& page_element_drawer_json, const cv::Size& page_size) : improc::PageElementDrawer()
{
    this->Load(factory,page_element_drawer_json,page_size);
}

improc::PageElementDrawer& improc::PageElementDrawer::Load(const improc::DrawerFactory& factory, const Json::Value& page_element_drawer_json, const cv::Size& page_size) 
{
    IMPROC_DRAWER_LOGGER_TRACE("Creating page element drawer...");
    static const std::string kTopLeftKey = "top-left";
    static const std::string kStaticKey  = "static";
    static const std::string kContentKey = "content";
    if (page_element_drawer_json.isMember(kTopLeftKey) == false)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: Top left position missing.");
        throw improc::file_processing_error();
    }
    if (page_element_drawer_json.isMember(kStaticKey) == false)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: Page element must be static or dynamic.");
        throw improc::file_processing_error();
    }

    this->improc::ElementDrawer::Load(factory,page_element_drawer_json);
    this->top_left_ = improc::PageElementDrawer::ParsePoint(page_element_drawer_json[kTopLeftKey],page_size);
    this->static_   = improc::json::ReadElement<bool>(page_element_drawer_json[kStaticKey]);
    if (page_element_drawer_json.isMember(kContentKey) == true)
    {
        this->content_ = improc::json::ReadElement<std::string>(page_element_drawer_json[kContentKey]);
    }
    return (*this);
}

cv::Point improc::PageElementDrawer::ParsePoint(const Json::Value& point_json, const cv::Size& page_size)
{
    IMPROC_DRAWER_LOGGER_TRACE("Parsing point...");
    cv::Point point = improc::json::ReadElement<cv::Point>(point_json);
    improc::PageElementDrawer::ValidatePoint(point,page_size);
    return point;
}

void improc::PageElementDrawer::ValidatePoint(const cv::Point& point, const cv::Size& page_size)
{
    IMPROC_DRAWER_LOGGER_TRACE("Validating point...");
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

improc::PageElementDrawer& improc::PageElementDrawer::Allocate()
{
    IMPROC_DRAWER_LOGGER_TRACE("Allocating page element...");
    cv::Size element_size     = this->ElementDrawer::Draw(this->content_).size();
    this->element_box_.x      = this->top_left_.x;
    this->element_box_.y      = this->top_left_.y;
    this->element_box_.width  = element_size.width;
    this->element_box_.height = element_size.height;
    return (*this);
}

void improc::PageElementDrawer::Draw(cv::Mat& page_image, const std::optional<std::string>& message) const
{
    IMPROC_DRAWER_LOGGER_TRACE  ( "Drawing page element..." );
    IMPROC_DRAWER_LOGGER_DEBUG  ( "Drawing element in x = {}, y = {} with size width = {}, height = {}"
                                , this->element_box_.x, this->element_box_.y, this->element_box_.width, this->element_box_.width);
    if (this->static_ == false)
    {
        this->ElementDrawer::Draw(message).copyTo(page_image(this->element_box_));
    }
    else
    {
        this->ElementDrawer::Draw(this->content_).copyTo(page_image(this->element_box_));
    }
}

improc::PageElementDrawer& improc::PageElementDrawer::IncrementTopLeftBy(const cv::Point& increment_top_left, const cv::Size& page_size)
{
    IMPROC_DRAWER_LOGGER_TRACE("Incrementing top left position...");
    cv::Point top_left {this->top_left_.x + increment_top_left.x, this->top_left_.y + increment_top_left.y};
    improc::PageElementDrawer::ValidatePoint(top_left,page_size);
    this->top_left_ = top_left;
    return (*this);
}

std::list<improc::PageElementDrawer> improc::PageElementDrawer::IncrementTopLeftBy(std::list<improc::PageElementDrawer>&& page_elements, const cv::Point& increment_top_left, const cv::Size& page_size)
{
    IMPROC_DRAWER_LOGGER_TRACE("Incrementing top left position on page elements...");
    //TODO: Increase performance using execution policy
    std::for_each   ( page_elements.begin(),page_elements.end()
                    , [&increment_top_left,&page_size] (improc::PageElementDrawer& elem) {elem.IncrementTopLeftBy(increment_top_left,page_size);} );
    return page_elements;
}

bool improc::PageElementDrawer::is_element_static() const
{
    IMPROC_DRAWER_LOGGER_TRACE("Checking if element is static...");
    return this->static_;
}
