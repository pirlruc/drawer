#include <improc/drawer/engine/grid_drawer.hpp>

/**
 * @brief Construct a new improc::GridDrawer object
 */
improc::GridDrawer::GridDrawer(): improc::PageDrawer() {};

/**
 * @brief Construct a new improc::GridDrawer object
 * 
 * @param factory - factory with base drawer types
 * @param grid_drawer_json - configuration json for grid drawer
 */
improc::GridDrawer::GridDrawer(const improc::DrawerFactory& factory, const Json::Value& grid_drawer_json) : improc::GridDrawer()
{
    this->Load(std::move(factory),std::move(grid_drawer_json));
}

/**
 * @brief Load configuration for a improc::GridDrawer object
 * 
 * @param factory - factory with base drawer types
 * @param grid_drawer_json - configuration json for grid drawer
 */
improc::GridDrawer& improc::GridDrawer::Load(const improc::DrawerFactory& factory, const Json::Value& grid_drawer_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Creating grid drawer...");
    static const std::string kGridNumberKey  = "number";
    static const std::string kGridSpacingKey = "spacing";
    static const std::string kGridCellKey    = "cell-content";
    if (grid_drawer_json.isMember(kGridNumberKey) == false)
    {
        std::string error_message = fmt::format("Key {} is missing from grid drawer json. This key corresponds to the number of elements in grid",kGridNumberKey);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::json_error(std::move(error_message));
    }
    if (grid_drawer_json.isMember(kGridSpacingKey) == false)
    {
        std::string error_message = fmt::format("Key {} is missing from grid drawer json",kGridSpacingKey);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: " + error_message);
        throw improc::json_error(std::move(error_message));
    }
    if (grid_drawer_json.isMember(kGridCellKey) == false)
    {
        std::string error_message = fmt::format("Key {} is missing from grid drawer json",kGridCellKey);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_03: " + error_message);
        throw improc::json_error(std::move(error_message));
    }
    cv::Point grid_number  = improc::GridDrawer::ParseGridNumber (grid_drawer_json[kGridNumberKey] );
    cv::Point grid_spacing = improc::GridDrawer::ParseGridSpacing(grid_drawer_json[kGridSpacingKey]);
    
    improc::PageDrawer cell {std::move(factory), grid_drawer_json[kGridCellKey]};
    cv::Size cell_size = cell.get_page_size();
    this->page_size_   = cv::Size   ( grid_number.x * cell_size.width  + (grid_number.x - 1) * grid_spacing.x
                                    , grid_number.y * cell_size.height + (grid_number.y - 1) * grid_spacing.y );
    this->elements_.clear();
    for (size_t cell_idx_x = 0; cell_idx_x < grid_number.x; cell_idx_x++)
    {
        int top_left_x = cell_idx_x * cell_size.width + cell_idx_x * grid_spacing.x;
        for (size_t cell_idx_y = 0; cell_idx_y < grid_number.y; cell_idx_y++)
        {
            int top_left_y = cell_idx_y * cell_size.height + cell_idx_y * grid_spacing.y;
            std::list<improc::PageElementDrawer> grid_elements = improc::PageElementDrawer::IncrementTopLeftBy(cell.get_page_elements(),cv::Point(top_left_x,top_left_y),this->page_size_);
            this->elements_.insert(this->elements_.end(),grid_elements.begin(),grid_elements.end());
        }
    }
    return (*this);
}

/**
 * @brief Parse and validate grid number
 * 
 * @param grid_number_json - configuration json for grid number
 */
cv::Point improc::GridDrawer::ParseGridNumber(const Json::Value& grid_number_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Parsing grid number...");
    cv::Point point = improc::json::ReadElement<cv::Point>(std::move(grid_number_json));
    if (improc::GridDrawer::IsNumberValid(point.x) == false)
    {
        std::string error_message = fmt::format("Number of cells in x should be greater than zero. {} cells were gave",point.x);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::value_error(std::move(error_message));
    }
    if (improc::GridDrawer::IsNumberValid(point.y) == false)
    {
        std::string error_message = fmt::format("Number of cells in y should be greater than zero. {} cells were gave",point.y);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: " + error_message);
        throw improc::value_error(std::move(error_message));
    }
    return point;
}

/**
 * @brief Parse and validate grid spacing
 * 
 * @param grid_spacing_json - configuration json for grid spacing
 */
cv::Point improc::GridDrawer::ParseGridSpacing(const Json::Value& grid_spacing_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Parsing grid spacing...");
    cv::Point point = improc::json::ReadElement<cv::Point>(std::move(grid_spacing_json));
    if (improc::GridDrawer::IsSpacingValid(point.x) == false)
    {
        std::string error_message = fmt::format("Spacing in x should be positive. {} x-spacing was gave",point.x);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::value_error(std::move(error_message));
    }
    if (improc::GridDrawer::IsSpacingValid(point.y) == false)
    {
        std::string error_message = fmt::format("Spacing in y should be positive. {} y-spacing was gave",point.y);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: " + error_message);
        throw improc::value_error(std::move(error_message));
    }
    return point;
}

/**
 * @brief Validate if grid number is valid
 * 
 * @param number - number of grid elements in x or y
 * @return true if number > 0
 */
inline bool improc::GridDrawer::IsNumberValid(int number)
{
    return number > 0;
}

/**
 * @brief Validate if grid spacing is valid
 * 
 * @param spacing - spacing between grid elements in x or y
 * @return true if spacing >= 0
 */
inline bool improc::GridDrawer::IsSpacingValid(int spacing)
{
    return spacing >= 0;
}

/**
 * @brief Allocate and initialize grid drawer
 */
improc::GridDrawer& improc::GridDrawer::Allocate()
{
    IMPROC_DRAWER_LOGGER_TRACE("Allocating grid...");
    this->improc::PageDrawer::Allocate();
    return (*this);
}
