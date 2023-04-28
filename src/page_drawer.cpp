#include <improc/drawer/engine/page_drawer.hpp>

/**
 * @brief Construct a new improc::PageDrawer object
 */
improc::PageDrawer::PageDrawer(): elements_(std::list<improc::PageElementDrawer>()) 
                                , page_size_(cv::Size())
                                , page_image_(cv::Mat()) {};

/**
 * @brief Construct a new improc::PageDrawer object
 * 
 * @param factory - factory with base drawer types
 * @param page_drawer_json - configuration json for page drawer
 */
improc::PageDrawer::PageDrawer(const improc::DrawerFactory& factory, const Json::Value& page_drawer_json) : improc::PageDrawer()
{
    this->Load(std::move(factory),std::move(page_drawer_json));
}

/**
 * @brief Load configuration for a improc::PageDrawer object
 * 
 * @param factory - factory with base drawer types
 * @param page_drawer_json - configuration json for page drawer
 */
improc::PageDrawer& improc::PageDrawer::Load(const improc::DrawerFactory& factory, const Json::Value& page_drawer_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Creating page drawer...");
    static const std::string kPageSizeKey = "page-size";
    static const std::string kElementsKey = "elements";
    if (page_drawer_json.isMember(kPageSizeKey) == false)
    {
        std::string error_message = fmt::format("Key {} is missing from page drawer json",kPageSizeKey);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::json_error(std::move(error_message));
    }
    this->page_size_ = improc::json::ReadPositiveSize<cv::Size>(page_drawer_json[kPageSizeKey]);

    if (page_drawer_json.isMember(kElementsKey) == true)
    {
        this->elements_.clear();
        if (page_drawer_json[kElementsKey].isArray() == true)
        {
            this->elements_ = std::list<improc::PageElementDrawer>(page_drawer_json[kElementsKey].size());
            std::transform  ( page_drawer_json[kElementsKey].begin(), page_drawer_json[kElementsKey].end(), this->elements_.begin()
                            , [this,&factory] (const Json::Value& json_elem) -> improc::PageElementDrawer {return improc::PageElementDrawer(factory,std::move(json_elem),this->page_size_);});
        }
        else
        {
            this->elements_.push_back(improc::PageElementDrawer(std::move(factory),page_drawer_json[kElementsKey],this->page_size_));
        }
    }
    return (*this);
}

/**
 * @brief Allocate and initialize page image
 */
improc::PageDrawer& improc::PageDrawer::Allocate()
{
    IMPROC_DRAWER_LOGGER_TRACE("Allocating page...");
    this->page_image_ = cv::Mat(this->page_size_.height,this->page_size_.width,improc::BaseDrawer::kImageDataType,improc::BaseDrawer::kWhiteValue);
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

/**
 * @brief Draw page
 * 
 * @param context - list of messages to be considered in page
 * @return cv::Mat - page image with page elements drawed
 */
cv::Mat improc::PageDrawer::Draw(const std::list<std::optional<improc::DrawerVariant>>& context)
{
    IMPROC_DRAWER_LOGGER_TRACE("Drawing page...");
    if (this->elements_.size() != context.size())
    {
        std::string error_message = fmt::format ( "Number of elements in context ({}) different than the number of elements in page ({})"
                                                , context.size(), this->elements_.size() );
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::value_error(std::move(error_message));
    }

    std::vector<bool> dummy = std::vector<bool>(this->elements_.size());
    // TODO: Implement with foreach whenever the second iterator is available on STL
    std::transform  ( this->elements_.begin(),this->elements_.end(),context.begin(),dummy.begin()
                    , [this] (const improc::PageElementDrawer& elem, const std::optional<improc::DrawerVariant>& message) -> bool
                        {
                            if (elem.is_element_static() == false)
                            {
                                elem.Draw(this->page_image_,std::move(message));
                            }
                            return true;
                        } 
                    );
    return this->page_image_;
}

/**
 * @brief Verify page
 * 
 * @param context - list of messages to be considered in page
 * @return bool - true if page is correct, false otherwise.
 */
bool improc::PageDrawer::Verify(const std::list<std::optional<improc::DrawerVariant>>& context)
{
    IMPROC_DRAWER_LOGGER_TRACE("Verifying page...");
    if (this->elements_.size() != context.size())
    {
        std::string error_message = fmt::format ( "Number of elements in context ({}) different than the number of elements in page ({})"
                                                , context.size(), this->elements_.size() );
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::value_error(std::move(error_message));
    }

    std::vector<bool> are_valid = std::vector<bool>(this->elements_.size());
    // TODO: Implement with foreach whenever the second iterator is available on STL
    std::transform  ( this->elements_.begin(),this->elements_.end(),context.begin(),are_valid.begin()
                    , [this] (const improc::PageElementDrawer& elem, const std::optional<improc::DrawerVariant>& message) -> bool
                        {
                            return elem.Verify(this->page_image_,std::move(message));
                        } 
                    );
    return std::all_of(are_valid.begin(),are_valid.end(),[] (bool is_valid) {return is_valid;});
}

/**
 * @brief Verify page
 * 
 * @param page_image - page image to be verified
 * @param context - list of messages to be considered in page
 * @return bool - true if page is correct, false otherwise.
 */
bool improc::PageDrawer::Verify(const cv::Mat& page_image,const std::list<std::optional<improc::DrawerVariant>>& context)
{
    IMPROC_DRAWER_LOGGER_TRACE("Verifying page given as input...");
    return this->set_page_image(std::move(page_image)).Verify(std::move(context));
}

/**
 * @brief Validate and set page image
 * 
 * @param page_image - page image data
 */
improc::PageDrawer& improc::PageDrawer::set_page_image(const cv::Mat& page_image)
{
    IMPROC_DRAWER_LOGGER_TRACE("Setting page image...");
    if (this->page_image_.empty() == true)
    {
        std::string error_message = "Please allocate page drawer before modifying page image";
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::processing_flow_error(std::move(error_message));
    }

    if (this->page_image_.size() != page_image.size())
    {
        std::string error_message = fmt::format ( "Page image (w={},h={}) should have the same size of allocated page image (w={},h={})"
                                                , page_image.size().width, page_image.size().height
                                                , this->page_image_.size().width, this->page_image_.size().height );
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: " + error_message);
        throw improc::value_error(std::move(error_message));
    }
    this->page_image_ = std::move(page_image);
    return (*this);
}
