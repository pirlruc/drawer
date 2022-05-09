#include <improc/drawer/page_drawer.hpp>

improc::PageDrawer::PageDrawer(): elements_(std::vector<improc::PageElementDrawer>()) 
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
    this->page_size_ = improc::ElementDrawer::ParseSize(page_drawer_json[kPageSizeKey]);

    if (page_drawer_json.isMember(kElementsKey) == true)
    {
        this->elements_.clear();
        if (page_drawer_json[kElementsKey].isArray() == true)
        {
            this->elements_ = std::vector<improc::PageElementDrawer>(page_drawer_json[kElementsKey].size());
            std::transform  ( page_drawer_json[kElementsKey].begin(), page_drawer_json[kElementsKey].end(), this->elements_.begin()
                            , [this,&factory] (const Json::Value& elem) -> improc::PageElementDrawer {return improc::PageElementDrawer(factory,elem,this->page_size_);} );
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
    this->page_image_ = 255 * cv::Mat::ones(this->page_size_.height,this->page_size_.width,CV_8UC1);
    std::for_each   ( this->elements_.begin(),this->elements_.end()
                    , [] (improc::PageElementDrawer& elem) {elem.Allocate();} );
    return (*this);
}

improc::PageDrawer& improc::PageDrawer::Draw()
{
    IMPROC_DRAWER_LOGGER_TRACE("Drawing page...");
    std::for_each   ( this->elements_.begin(),this->elements_.end()
                    , [this] (const improc::PageElementDrawer& elem) {elem.Draw(this->page_image_);} );
    return (*this);
}

cv::Size improc::PageDrawer::get_page_size() const
{
    IMPROC_DRAWER_LOGGER_TRACE("Obtaining page size...");
    return this->page_size_;
}

std::vector<improc::PageElementDrawer> improc::PageDrawer::get_page_elements() const
{
    IMPROC_DRAWER_LOGGER_TRACE("Obtaining page elements...");
    return this->elements_;
}
