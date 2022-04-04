#include <improc/drawer/grid_drawer.hpp>

improc::GridDrawer::GridDrawer(): improc::PageDrawer() {};

improc::GridDrawer::GridDrawer(const improc::DrawerFactory& factory, const Json::Value& grid_drawer_json) : improc::GridDrawer()
{
    this->Load(factory,grid_drawer_json);
}

improc::GridDrawer& improc::GridDrawer::Load(const improc::DrawerFactory& factory, const Json::Value& grid_drawer_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Creating grid drawer...");
    static const std::string kGridNumberKey  = "number";
    static const std::string kGridSpacingKey = "spacing";
    static const std::string kGridCellKey    = "cell-content";
    if (grid_drawer_json.isMember(kGridNumberKey) == false)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: Number of elements in grid missing.");
        throw improc::file_processing_error();
    }
    if (grid_drawer_json.isMember(kGridSpacingKey) == false)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: Grid spacing missing.");
        throw improc::file_processing_error();
    }
    if (grid_drawer_json.isMember(kGridCellKey) == false)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_03: Grid cell content missing.");
        throw improc::file_processing_error();
    }
    cv::Point grid_number  = improc::GridDrawer::ParseGridNumber (grid_drawer_json[kGridNumberKey] );
    cv::Point grid_spacing = improc::GridDrawer::ParseGridSpacing(grid_drawer_json[kGridSpacingKey]);
    
    improc::PageDrawer cell {factory, grid_drawer_json[kGridCellKey]};
    cv::Size cell_size = cell.get_page_size();
    cv::Size grid_size = cv::Size   ( grid_number.x * cell_size.width  + (grid_number.x - 1) * grid_spacing.x
                                    , grid_number.y * cell_size.height + (grid_number.y - 1) * grid_spacing.y );
    this->page_size_   = grid_size;

    std::vector<improc::PageElementDrawer> cell_elements = cell.get_page_elements();
    this->elements_.clear();
    for (size_t cell_idx_x = 0; cell_idx_x < grid_number.x; cell_idx_x++)
    {
        int top_left_x = cell_idx_x * cell_size.width + cell_idx_x * grid_spacing.x;
        for (size_t cell_idx_y = 0; cell_idx_y < grid_number.y; cell_idx_y++)
        {
            int top_left_y = cell_idx_y * cell_size.height + cell_idx_y * grid_spacing.y;
            std::vector<improc::PageElementDrawer> grid_elements = improc::PageElementDrawer::IncrementTopLeftBy(cell_elements,cv::Point(top_left_x,top_left_y),grid_size);
            this->elements_.insert(this->elements_.end(),grid_elements.begin(),grid_elements.end());
        }
    }
    return (*this);
}

cv::Point improc::GridDrawer::ParseGridNumber(const Json::Value& grid_number_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Parsing grid number...");
    cv::Point point = improc::json::ReadElement<cv::Point>(grid_number_json);
    if (improc::GridDrawer::IsNumberValid(point.x) == false)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: Number of cells in x should be greater than zero");
        throw improc::file_processing_error();
    }
    if (improc::GridDrawer::IsNumberValid(point.y) == false)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: Number of cells in y should be greater than zero");
        throw improc::file_processing_error();
    }
    return point;
}

cv::Point improc::GridDrawer::ParseGridSpacing(const Json::Value& grid_spacing_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Parsing grid spacing...");
    cv::Point point = improc::json::ReadElement<cv::Point>(grid_spacing_json);
    if (improc::GridDrawer::IsSpacingValid(point.x) == false)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: Spacing in x should be positive");
        throw improc::file_processing_error();
    }
    if (improc::GridDrawer::IsSpacingValid(point.y) == false)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: Spacing in y should be positive");
        throw improc::file_processing_error();
    }
    return point;
}

bool improc::GridDrawer::IsNumberValid(int number)
{
    IMPROC_DRAWER_LOGGER_TRACE("Validating number...");
    bool result = true;
    if (number <= 0)
    {
        result = false;
    }
    return result;
}

bool improc::GridDrawer::IsSpacingValid(int spacing)
{
    IMPROC_DRAWER_LOGGER_TRACE("Validating spacing...");
    bool result = true;
    if (spacing < 0)
    {
        result = false;
    }
    return result;
}

improc::GridDrawer& improc::GridDrawer::Allocate()
{
    IMPROC_DRAWER_LOGGER_TRACE("Allocating grid...");
    this->improc::PageDrawer::Allocate();
    return (*this);
}

improc::GridDrawer& improc::GridDrawer::Draw()
{
    IMPROC_DRAWER_LOGGER_TRACE("Drawing grid...");
    this->improc::PageDrawer::Draw();
    return (*this);
}
