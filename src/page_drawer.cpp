#include <improc/drawer/engine/page_drawer.hpp>

improc::PageDrawer::PageDrawer(): elements_(std::list<improc::PageElementDrawer>()) 
                                , page_size_(cv::Size())
                                , page_image_(cv::Mat()) {};

improc::PageDrawer::PageDrawer(const improc::DrawerFactory& factory, const Json::Value& page_drawer_json) : improc::PageDrawer()
{
    this->Load(factory,page_drawer_json);
}

improc::PageDrawer& improc::PageDrawer::Load(const improc::DrawerFactory& factory, const Json::Value& page_drawer_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Creating page drawer...");
    static const std::string kPageSizeKey = "page-size";
    static const std::string kElementsKey = "elements";
    if (page_drawer_json.isMember(kPageSizeKey) == false)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: Page size missing.");
        throw improc::file_processing_error();
    }
    this->page_size_ = improc::json::ReadPositiveSize<cv::Size>(page_drawer_json[kPageSizeKey]);

    if (page_drawer_json.isMember(kElementsKey) == true)
    {
        this->elements_.clear();
        if (page_drawer_json[kElementsKey].isArray() == true)
        {
            //TODO: Increase performance using execution policy
            this->elements_ = std::list<improc::PageElementDrawer>(page_drawer_json[kElementsKey].size());
            std::transform  ( page_drawer_json[kElementsKey].begin(), page_drawer_json[kElementsKey].end(), this->elements_.begin()
                            , [this,&factory] (const Json::Value& json_elem) -> improc::PageElementDrawer {return improc::PageElementDrawer(factory,json_elem,this->page_size_);});
        }
        else
        {
            this->elements_.push_back(improc::PageElementDrawer(factory,page_drawer_json[kElementsKey],this->page_size_));
        }
    }
    return (*this);
}

improc::PageDrawer& improc::PageDrawer::Allocate()
{
    IMPROC_DRAWER_LOGGER_TRACE("Allocating page...");
    this->page_image_ = 255 * cv::Mat::ones(this->page_size_.height,this->page_size_.width,improc::PageDrawer::kImageDataType);
    //TODO: Increase performance using execution policy
    std::for_each   ( this->elements_.begin(),this->elements_.end()
                    , [this] (improc::PageElementDrawer& elem) 
                        {
                            elem.Allocate();
                            if (elem.is_element_static() == true)
                            {
                                elem.Draw(this->page_image_);
                            }
                        } 
                    );
    return (*this);
}

cv::Mat improc::PageDrawer::Draw(const std::list<std::optional<std::string>>& context)
{
    IMPROC_DRAWER_LOGGER_TRACE("Drawing page...");
    if (this->elements_.size() != context.size())
    {
        IMPROC_DRAWER_LOGGER_ERROR  ( "ERROR_02: Number of elements in context ({}) different than the number of elements in page ({})."
                                    , context.size(), this->elements_.size() );
        throw improc::context_elem_diff_page_elem();
    }

    std::vector<bool> dummy = std::vector<bool>(this->elements_.size());
    // TODO: Increase performance using execution policy
    // TODO: Implement with foreach whenever the second iterator is available on STL
    std::transform  ( this->elements_.begin(),this->elements_.end(),context.begin(),dummy.begin()
                    , [this] (const improc::PageElementDrawer& elem, const std::optional<std::string>& message) -> bool
                        {
                            if (elem.is_element_static() == false)
                            {
                                elem.Draw(this->page_image_,message);
                            }
                            return true;
                        } 
                    );
    return this->page_image_;
}

bool improc::PageDrawer::Verify(const std::list<std::optional<std::string>>& context)
{
    IMPROC_DRAWER_LOGGER_TRACE("Verifying page...");
    if (this->elements_.size() != context.size())
    {
        IMPROC_DRAWER_LOGGER_ERROR  ( "ERROR_03: Number of elements in context ({}) different than the number of elements in page ({})."
                                    , context.size(), this->elements_.size() );
        throw improc::context_elem_diff_page_elem();
    }

    std::vector<bool> are_valid = std::vector<bool>(this->elements_.size());
    // TODO: Increase performance using execution policy
    // TODO: Implement with foreach whenever the second iterator is available on STL
    std::transform  ( this->elements_.begin(),this->elements_.end(),context.begin(),are_valid.begin()
                    , [this] (const improc::PageElementDrawer& elem, const std::optional<std::string>& message) -> bool
                        {
                            return elem.Verify(this->page_image_,message);
                        } 
                    );
    return std::all_of(are_valid.begin(),are_valid.end(),[] (bool is_valid) {return is_valid;});
}

bool improc::PageDrawer::Verify(const cv::Mat& page_image,const std::list<std::optional<std::string>>& context)
{
    IMPROC_DRAWER_LOGGER_TRACE("Verifying page given as input...");
    return this->set_page_image(page_image).Verify(context);
}

improc::PageDrawer& improc::PageDrawer::set_page_image(const cv::Mat& page_image)
{
    IMPROC_DRAWER_LOGGER_TRACE("Setting page image...");
    if (this->page_image_.empty() == true)
    {
        IMPROC_DRAWER_LOGGER_ERROR  ("ERROR_04: Please allocate page drawer before modifying page image.");
        throw improc::page_drawer_not_allocated();
    }

    if (this->page_image_.size() != page_image.size())
    {
        IMPROC_DRAWER_LOGGER_ERROR  ("ERROR_05: Page image should have the same size of allocated page image.");
        throw improc::invalid_page_image();
    }
    this->page_image_ = page_image;
    return (*this);
}

cv::Size improc::PageDrawer::get_page_size() const
{
    IMPROC_DRAWER_LOGGER_TRACE("Obtaining page size...");
    return this->page_size_;
}

std::list<improc::PageElementDrawer> improc::PageDrawer::get_page_elements() const
{
    IMPROC_DRAWER_LOGGER_TRACE("Obtaining page elements...");
    return this->elements_;
}
