#include <improc/drawer/layout_drawer.hpp>

/**
 * @brief Construct a new improc::LayoutDrawer object
 */
improc::LayoutDrawer::LayoutDrawer(): improc::PageDrawer() {};

/**
 * @brief Construct a new improc::LayoutDrawer object
 * 
 * @param factory - factory with base drawer types
 * @param layout_drawer_json - configuration json for layout drawer
 */
improc::LayoutDrawer::LayoutDrawer(const improc::DrawerFactory& factory, const Json::Value& layout_drawer_json) : improc::LayoutDrawer()
{
    this->Load(std::move(factory),std::move(layout_drawer_json));
}

/**
 * @brief Load configuration for a improc::LayoutDrawer object
 * 
 * @param factory - factory with base drawer types
 * @param layout_drawer_json - configuration json for layout drawer
 */
improc::LayoutDrawer& improc::LayoutDrawer::Load(const improc::DrawerFactory& factory, const Json::Value& layout_drawer_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Creating layout drawer...");
    this->elements_.clear();
    if (layout_drawer_json.isArray() == true)
    {
        std::for_each   ( layout_drawer_json.begin(), layout_drawer_json.end()
                        , [this,&factory] (const Json::Value& elem) {this->ParsePageTypeDrawer(factory,std::move(elem));} );
    }
    else
    {
        improc::LayoutDrawer::ParsePageTypeDrawer(std::move(factory),std::move(layout_drawer_json));
    }
    return (*this);
}

/**
 * @brief Parse grid and page type drawers
 * 
 * @param factory - factory with base drawer types
 * @param page_drawer_type_json - configuration json for page type drawer
 */
improc::LayoutDrawer& improc::LayoutDrawer::ParsePageTypeDrawer(const improc::DrawerFactory& factory, const Json::Value& page_drawer_type_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Parsing page type drawer...");
    static const std::string kPageDrawerTypeKey = "page-drawer-type";
    static const std::string kTopLeftKey        = "top-left";
    if (page_drawer_type_json.isMember(kPageDrawerTypeKey) == false)
    {
        std::string error_message = fmt::format("Key {} is missing from layout drawer json",kPageDrawerTypeKey);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::json_error(std::move(error_message));
    }
    if (page_drawer_type_json.isMember(kTopLeftKey) == false)
    {
        std::string error_message = fmt::format("Key {} is missing from layout drawer json",kTopLeftKey);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: " + error_message);
        throw improc::json_error(std::move(error_message));
    }

    improc::PageDrawerType page_drawer_type {improc::json::ReadElement<std::string>(page_drawer_type_json[kPageDrawerTypeKey])};
    cv::Point top_left    = improc::LayoutDrawer::ParsePoint(page_drawer_type_json[kTopLeftKey]);
    std::shared_ptr<improc::PageDrawer> page_drawer {};
    if (page_drawer_type == improc::PageDrawerType::kPageDrawer)
    {
        page_drawer = std::make_shared<improc::PageDrawer>(improc::PageDrawer(std::move(factory),std::move(page_drawer_type_json)));
    }
    else if (page_drawer_type == improc::PageDrawerType::kGridDrawer)
    {
        page_drawer = std::make_shared<improc::GridDrawer>(improc::GridDrawer(std::move(factory),std::move(page_drawer_type_json)));
    }
    else
    {
        std::string error_message = fmt::format("Page drawer type {} not implemented",std::move(page_drawer_type));
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_03: " + error_message);
        throw improc::value_error(std::move(error_message));
    }

    this->CorrectLayoutSize(top_left,page_drawer->get_page_size());
    std::list<improc::PageElementDrawer> page_elements = improc::PageElementDrawer::IncrementTopLeftBy(page_drawer->get_page_elements(),std::move(top_left),this->page_size_);
    this->elements_.insert(this->elements_.end(),page_elements.begin(),page_elements.end());
    return (*this);
}

/**
 * @brief Correct page size for layout according with page drawer components
 * 
 * @param top_left - page drawer top left pixel position
 * @param page_drawer_size - page drawer size in pixels
 */
improc::LayoutDrawer& improc::LayoutDrawer::CorrectLayoutSize(const cv::Point& top_left, const cv::Size& page_drawer_size)
{
    IMPROC_DRAWER_LOGGER_TRACE("Correcting layout size...");
    cv::Size  layout_size = cv::Size(top_left.x + page_drawer_size.width, top_left.y + page_drawer_size.height);
    if (this->page_size_.width  < layout_size.width)
    {
        this->page_size_.width  = layout_size.width;
    }
    if (this->page_size_.height < layout_size.height)
    {
        this->page_size_.height = layout_size.height;
    }
    return (*this);
}

/**
 * @brief Parse and validate top left position for layout
 * 
 * @param point_json - configuration json for layout top left position in pixels
 * @return cv::Point - layout top left position
 */
cv::Point improc::LayoutDrawer::ParsePoint(const Json::Value& point_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Parsing point...");
    cv::Point point = improc::json::ReadElement<cv::Point>(std::move(point_json));
    if (improc::LayoutDrawer::IsPixelPositionValid(point.x) == false)
    {
        std::string error_message = fmt::format("Invalid x-position. It should be greater or equal than 0. {} x-position was gave",point.x);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::value_error(std::move(error_message));
    }
    if (improc::LayoutDrawer::IsPixelPositionValid(point.y) == false)
    {
        std::string error_message = fmt::format("Invalid y-position. It should be greater or equal than 0. {} y-position was gave",point.y);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: " + error_message);
        throw improc::value_error(std::move(error_message));
    }
    return point;
}

/**
 * @brief Validate if pixel position is correct
 * 
 * @param pixel_position - x or y pixel position in pixels
 * @return true if pixel position >= 0
 */
inline bool improc::LayoutDrawer::IsPixelPositionValid(int pixel_position)
{
    return pixel_position >= 0;
}

/**
 * @brief Allocate and initialize layout drawer
 */
improc::LayoutDrawer& improc::LayoutDrawer::Allocate()
{
    IMPROC_DRAWER_LOGGER_TRACE("Allocating layout...");
    this->improc::PageDrawer::Allocate();
    return (*this);
}

/**
 * @brief Draw layout
 * 
 * @param context - list of messages to be considered in layout
 * @return cv::Mat - page image with layout elements drawed
 */
cv::Mat improc::LayoutDrawer::Draw(const std::list<std::optional<improc::DrawerVariant>>& context)
{
    IMPROC_DRAWER_LOGGER_TRACE("Drawing layout...");
    return this->improc::PageDrawer::Draw(std::move(context));
}

/**
 * @brief Verify layout
 * 
 * @param context - list of messages to be considered in layout
 * @return bool - true if layout is correct, false otherwise.
 */
bool improc::LayoutDrawer::Verify(const std::list<std::optional<improc::DrawerVariant>>& context)
{
    IMPROC_DRAWER_LOGGER_TRACE("Verifying layout...");
    return this->improc::PageDrawer::Verify(std::move(context));
}

/**
 * @brief Verify layout
 * 
 * @param page_image - page image to be verified
 * @param context - list of messages to be considered in layout
 * @return bool - true if layout is correct, false otherwise.
 */
bool improc::LayoutDrawer::Verify(const cv::Mat& page_image,const std::list<std::optional<improc::DrawerVariant>>& context)
{
    IMPROC_DRAWER_LOGGER_TRACE("Verifying layout given as input...");
    return this->improc::PageDrawer::Verify(std::move(page_image),std::move(context));
}
