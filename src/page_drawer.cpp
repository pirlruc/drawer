#include <improc/drawer/page_drawer.hpp>

improc::PageDrawer::PageDrawer(): elements_(std::vector<improc::PageElementDrawer>()) 
                                , page_image_(cv::Mat()) {};

improc::PageDrawer::PageDrawer(const improc::DrawerFactory& factory, const Json::Value& page_drawer_json)
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
    cv::Size page_size = improc::PageDrawer::ParseSize(page_drawer_json[kPageSizeKey]);
    this->page_image_  = 255 * cv::Mat::ones(page_size.height,page_size.width,CV_8UC1);

    if (page_drawer_json.isMember(kElementsKey) == true)
    {
        this->elements_.clear();
        if (page_drawer_json[kElementsKey].isArray() == true)
        {
            this->elements_ = std::vector<improc::PageElementDrawer>(page_drawer_json[kElementsKey].size());
            std::transform  ( page_drawer_json[kElementsKey].begin(), page_drawer_json[kElementsKey].end(), this->elements_.begin()
                            , [&factory, &page_size] (const Json::Value& elem) -> improc::PageElementDrawer {return improc::PageElementDrawer(factory,elem,page_size);} );
        }
        else
        {
            this->elements_.push_back(improc::PageElementDrawer(factory,page_drawer_json[kElementsKey],page_size));
        }
    }
    return (*this);
}

cv::Size improc::PageDrawer::ParseSize(const Json::Value& size_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Parsing page size...");
    cv::Size size = improc::json::ReadElement<cv::Size>(size_json);
    if (improc::PageDrawer::IsLengthValid(size.width) == false)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: Width should be greater than zero");
        throw improc::file_processing_error();
    }
    if (improc::PageDrawer::IsLengthValid(size.height) == false)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: Height should be greater than zero");
        throw improc::file_processing_error();
    }
    return size;
}

bool improc::PageDrawer::IsLengthValid(int length)
{
    IMPROC_DRAWER_LOGGER_TRACE("Validating size length...");
    bool result = true;
    if (length <= 0)
    {
        result = false;
    }
    return result;
}

improc::PageDrawer& improc::PageDrawer::Draw()
{
    IMPROC_DRAWER_LOGGER_TRACE("Drawing page...");
    cv::Mat page_pointer = this->page_image_;
    std::for_each   ( this->elements_.begin(),this->elements_.end()
                    , [&page_pointer] (const improc::PageElementDrawer& elem) {elem.Draw(page_pointer);} );
    return (*this);
}
