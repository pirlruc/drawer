#include <improc/drawer/engine/page_element_drawer.hpp>

/**
 * @brief Construct a new improc::PageElementDrawer object
 */
improc::PageElementDrawer::PageElementDrawer()  : improc::ElementDrawer()
                                                , top_left_(cv::Point())
                                                , element_box_(cv::Rect()) 
                                                , static_(false) 
                                                , content_(std::optional<improc::DrawerVariant>()) {};

/**
 * @brief Construct a new improc::PageElementDrawer object
 * 
 * @param factory - factory with base drawer types
 * @param page_element_drawer_json - configuration json for page element drawer
 * @param page_size - page size in pixels
 */
improc::PageElementDrawer::PageElementDrawer(const improc::DrawerFactory& factory, const Json::Value& page_element_drawer_json, const cv::Size& page_size) : improc::PageElementDrawer()
{
    this->Load(std::move(factory),std::move(page_element_drawer_json),std::move(page_size));
}

/**
 * @brief Load configuration for a improc::PageElementDrawer object
 * 
 * @param factory - factory with base drawer types
 * @param page_element_drawer_json - configuration json for page element drawer
 * @param page_size - page size in pixels
 */
improc::PageElementDrawer& improc::PageElementDrawer::Load(const improc::DrawerFactory& factory, const Json::Value& page_element_drawer_json, const cv::Size& page_size) 
{
    IMPROC_DRAWER_LOGGER_TRACE("Creating page element drawer...");
    static const std::string kTopLeftKey = "top-left";
    static const std::string kStaticKey  = "static";
    static const std::string kContentKey = "content";
    if (page_element_drawer_json.isMember(kTopLeftKey) == false)
    {
        std::string error_message = fmt::format("Key {} is missing from page element drawer json",kTopLeftKey);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::json_error(std::move(error_message));
    }
    if (page_element_drawer_json.isMember(kStaticKey) == false)
    {
        std::string error_message = fmt::format("Key {} is missing from page element drawer json",kStaticKey);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: " + error_message);
        throw improc::json_error(std::move(error_message));
    }

    this->improc::ElementDrawer::Load(std::move(factory),page_element_drawer_json);
    this->top_left_ = improc::PageElementDrawer::ParsePoint(page_element_drawer_json[kTopLeftKey],std::move(page_size));
    this->static_   = improc::json::ReadElement<bool>(page_element_drawer_json[kStaticKey]);
    if (page_element_drawer_json.isMember(kContentKey) == true)
    {
        this->content_ = improc::json::ReadElement<std::string>(page_element_drawer_json[kContentKey]);
    }
    return (*this);
}

/**
 * @brief Parse and validate point structure in the scope of page element drawer
 * 
 * @param point_json - configuration json for point in pixels
 * @param page_size - page size in pixels
 * @return cv::Point
 */
cv::Point improc::PageElementDrawer::ParsePoint(const Json::Value& point_json, const cv::Size& page_size)
{
    IMPROC_DRAWER_LOGGER_TRACE("Parsing point...");
    cv::Point point = improc::json::ReadElement<cv::Point>(std::move(point_json));
    improc::PageElementDrawer::ValidatePoint(point,std::move(page_size));
    return point;
}

/**
 * @brief Validate point in the scope of page element drawer
 * 
 * @param point - point in pixels
 * @param page_size - page size in pixels
 */
void improc::PageElementDrawer::ValidatePoint(const cv::Point& point, const cv::Size& page_size)
{
    IMPROC_DRAWER_LOGGER_TRACE("Validating point...");
    if (improc::PageElementDrawer::IsPixelPositionValid(point.x,page_size.width) == false)
    {
        std::string error_message = fmt::format("Invalid x-position for page element drawer top left point. It should be between 0 and {}. Position gave was {}",page_size.width,point.x);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::value_error(std::move(error_message));
    }
    if (improc::PageElementDrawer::IsPixelPositionValid(point.y,page_size.height) == false)
    {
        std::string error_message = fmt::format("Invalid y-position for page element drawer top left point. It should be between 0 and {}. Position gave was {}",page_size.height,point.y);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: " + error_message);
        throw improc::value_error(std::move(error_message));
    }
}

/**
 * @brief Validate pixel position
 * 
 * @param pixel_position - x or y pixel position
 * @param max_pixel_size - page width or height
 * @return true if pixel position between 0 and max_pixel_size
 */
inline bool improc::PageElementDrawer::IsPixelPositionValid(int pixel_position, int max_pixel_size)
{
    return pixel_position >= 0 && pixel_position < max_pixel_size;
}

/**
 * @brief Allocate page element
 */
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

/**
 * @brief Draw page element
 * 
 * @param page_image - page image to draw page element
 * @param message - message to be considered in page element
 * @return page_image
 */
void improc::PageElementDrawer::Draw(cv::Mat& page_image, const std::optional<improc::DrawerVariant>& message) const
{
    IMPROC_DRAWER_LOGGER_TRACE  ( "Drawing page element..." );
    if (this->element_box_.empty() == true)
    {
        std::string error_message = "Please allocate page element drawer before calling Draw method";
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::processing_flow_error(std::move(error_message));
    }

    IMPROC_DRAWER_LOGGER_DEBUG  ( "Drawing element in x = {}, y = {} with size width = {}, height = {}"
                                , this->element_box_.x, this->element_box_.y, this->element_box_.width, this->element_box_.height);
    if (this->static_ == false)
    {
        this->ElementDrawer::Draw(std::move(message)).copyTo(page_image(this->element_box_));
    }
    else
    {
        this->ElementDrawer::Draw(this->content_).copyTo(page_image(this->element_box_));
    }
}

/**
 * @brief Verify page element
 * 
 * @param page_image - page image with drawed page element
 * @param message - message to be considered in page element
 * @return bool - true if page element is correct, false otherwise.
 */
bool improc::PageElementDrawer::Verify(const cv::Mat& page_image, const std::optional<improc::DrawerVariant>& message) const
{
    IMPROC_DRAWER_LOGGER_TRACE  ( "Verifying page element..." );
    if (this->element_box_.empty() == true)
    {
        std::string error_message = "Please allocate page element drawer before calling Verify method";
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::processing_flow_error(std::move(error_message));
    }

    IMPROC_DRAWER_LOGGER_DEBUG  ( "Verifying element in x = {}, y = {} with size width = {}, height = {}"
                                , this->element_box_.x, this->element_box_.y, this->element_box_.width, this->element_box_.height);
    bool is_valid {};
    if (this->static_ == false)
    {
        is_valid = this->ElementDrawer::Verify(page_image(this->element_box_),std::move(message));
    }
    else
    {
        is_valid = this->ElementDrawer::Verify(page_image(this->element_box_),this->content_);
    }
    return is_valid;
}

/**
 * @brief Increment top left position of page element
 * 
 * @param increment_top_left - top left increment in pixels
 * @param page_size - page size in pixels
 */
improc::PageElementDrawer& improc::PageElementDrawer::IncrementTopLeftBy(const cv::Point& increment_top_left, const cv::Size& page_size)
{
    IMPROC_DRAWER_LOGGER_TRACE("Incrementing top left position...");
    cv::Point top_left {this->top_left_.x + increment_top_left.x, this->top_left_.y + increment_top_left.y};
    improc::PageElementDrawer::ValidatePoint(top_left,std::move(page_size));
    this->top_left_ = top_left;
    if (this->element_box_.empty() == false)
    {
        this->element_box_.x      = this->top_left_.x;
        this->element_box_.y      = this->top_left_.y;
    }
    return (*this);
}

/**
 * @brief Increment top left position of page elements
 * 
 * @param page_elements - page elements to increment top left position
 * @param increment_top_left - top left increment in pixels
 * @param page_size - page size in pixels
 * @return std::list<improc::PageElementDrawer> - page elements with top leftr position changed
 */
std::list<improc::PageElementDrawer> improc::PageElementDrawer::IncrementTopLeftBy(std::list<improc::PageElementDrawer>&& page_elements, const cv::Point& increment_top_left, const cv::Size& page_size)
{
    IMPROC_DRAWER_LOGGER_TRACE("Incrementing top left position on page elements...");
    std::for_each   ( page_elements.begin(),page_elements.end()
                    , [&increment_top_left,&page_size] (improc::PageElementDrawer& elem) {elem.IncrementTopLeftBy(increment_top_left,page_size);} );
    return page_elements;
}
