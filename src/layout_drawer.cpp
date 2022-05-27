#include <improc/drawer/layout_drawer.hpp>

improc::LayoutDrawer::LayoutDrawer(): improc::PageDrawer() {};

improc::LayoutDrawer::LayoutDrawer(const improc::DrawerFactory& factory, const Json::Value& layout_drawer_json) : improc::LayoutDrawer()
{
    this->Load(factory,layout_drawer_json);
}

improc::LayoutDrawer& improc::LayoutDrawer::Load(const improc::DrawerFactory& factory, const Json::Value& layout_drawer_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Creating layout drawer...");
    this->elements_.clear();
    if (layout_drawer_json.isArray() == true)
    {
        //TODO: Increase performance using execution policy
        std::for_each   ( layout_drawer_json.begin(), layout_drawer_json.end()
                        , [this,&factory] (const Json::Value& elem) {this->ParsePageTypeDrawer(factory,elem);} );
    }
    else
    {
        improc::LayoutDrawer::ParsePageTypeDrawer(factory,layout_drawer_json);
    }
    return (*this);
}

improc::LayoutDrawer& improc::LayoutDrawer::ParsePageTypeDrawer(const improc::DrawerFactory& factory, const Json::Value& page_drawer_type_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Parsing page type drawer...");
    static const std::string kPageDrawerTypeKey = "page-drawer-type";
    static const std::string kTopLeftKey        = "top-left";
    if (page_drawer_type_json.isMember(kPageDrawerTypeKey) == false)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: Page drawer type missing.");
        throw improc::file_processing_error();
    }
    if (page_drawer_type_json.isMember(kTopLeftKey) == false)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: Top left position missing.");
        throw improc::file_processing_error();
    }

    improc::PageDrawerType page_drawer_type {improc::json::ReadElement<std::string>(page_drawer_type_json[kPageDrawerTypeKey])};
    std::shared_ptr<improc::PageDrawer> page_drawer {};
    if (page_drawer_type == improc::PageDrawerType::kPageDrawer)
    {
        page_drawer = std::make_shared<improc::PageDrawer>(improc::PageDrawer(factory,page_drawer_type_json));
    }
    else if (page_drawer_type == improc::PageDrawerType::kGridDrawer)
    {
        page_drawer = std::make_shared<improc::GridDrawer>(improc::GridDrawer(factory,page_drawer_type_json));
    }
    else
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_03: Page drawer type not implemented.");
        throw improc::file_processing_error();
    }

    cv::Point top_left    = improc::LayoutDrawer::ParsePoint(page_drawer_type_json[kTopLeftKey]);
    this->CorrectLayoutSize(top_left,page_drawer->get_page_size());
    std::list<improc::PageElementDrawer> page_elements = improc::PageElementDrawer::IncrementTopLeftBy(page_drawer->get_page_elements(),top_left,this->page_size_);
    this->elements_.insert(this->elements_.end(),page_elements.begin(),page_elements.end());
    return (*this);
}

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

cv::Point improc::LayoutDrawer::ParsePoint(const Json::Value& point_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Parsing point...");
    cv::Point point = improc::json::ReadElement<cv::Point>(point_json);
    if (improc::LayoutDrawer::IsPixelPositionValid(point.x) == false)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: Invalid x-position. It should be greater or equal than 0.");
        throw improc::file_processing_error();
    }
    if (improc::LayoutDrawer::IsPixelPositionValid(point.y) == false)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: Invalid y-position. It should be greater or equal than 0.");
        throw improc::file_processing_error();
    }
    return point;
}

bool improc::LayoutDrawer::IsPixelPositionValid(int pixel_position)
{
    IMPROC_DRAWER_LOGGER_TRACE("Validating pixel position...");
    bool result = true;
    if (pixel_position < 0)
    {
        result = false;
    }
    return result;
}

improc::LayoutDrawer& improc::LayoutDrawer::Allocate()
{
    IMPROC_DRAWER_LOGGER_TRACE("Allocating layout...");
    this->improc::PageDrawer::Allocate();
    return (*this);
}

improc::LayoutDrawer& improc::LayoutDrawer::Draw(const std::list<std::optional<std::string>>& context)
{
    IMPROC_DRAWER_LOGGER_TRACE("Drawing layout...");
    this->improc::PageDrawer::Draw(context);
    return (*this);
}
