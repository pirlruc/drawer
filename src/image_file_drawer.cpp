#include <improc/drawer/drawer_types/image_file_drawer.hpp>

/**
 * @brief Construct a new improc::ImageFileDrawer object
 */
improc::ImageFileDrawer::ImageFileDrawer()  : improc::BaseDrawer()
                                            , image_data_(cv::Mat()) {};

/**
 * @brief Construct a new improc::ImageFileDrawer object
 * 
 * @param drawer_json - configuration json for image file drawer
 */
improc::ImageFileDrawer::ImageFileDrawer(const Json::Value& drawer_json) : improc::ImageFileDrawer() 
{
    this->Load(std::move(drawer_json));
};

/**
 * @brief Load configuration for a improc::ImageFileDrawer object
 * 
 * @param drawer_json - configuration json for image file drawer
 */
improc::ImageFileDrawer& improc::ImageFileDrawer::Load(const Json::Value& drawer_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Creating image file drawer...");
    static const std::string kImageFileKey = "image-filepath";
    if (drawer_json.isMember(kImageFileKey) == false) 
    {
        std::string error_message = fmt::format("Key {} is missing from image file drawer json",kImageFileKey);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::json_error(std::move(error_message));
    }
    improc::File image_file {drawer_json[kImageFileKey],improc::ApplicationContext::get()->get_application_folder()};
    this->image_data_ = cv::imread(image_file.get_filepath(),improc::ImageFileDrawer::kImageReadMode);
    return (*this);
};

/**
 * @brief Draw image file
 * 
 * @param message - message for image file
 * @return cv::Mat - image with content of image file
 */
cv::Mat improc::ImageFileDrawer::Draw(const std::optional<std::string>& message)
{
    IMPROC_DRAWER_LOGGER_TRACE("Drawing image file content...");
    return this->image_data_;
};

/**
 * @brief Verify image content
 * 
 * @param drawer_output - image file content
 * @param message - message for image file
 * @return bool - true if image content and image file content are the same, false otherwise.
 */
bool improc::ImageFileDrawer::Verify(const cv::Mat& drawer_output, const std::optional<std::string>& message)
{
    IMPROC_DRAWER_LOGGER_TRACE("Verifying image file content...");
    // TODO: Move this logic to an image class object
    if (drawer_output.size() != this->image_data_.size())
    {
        return false;
    }
    else if (drawer_output.empty() == true && this->image_data_.empty() == true)
    {
        return true;
    }
    else
    {
        return cv::norm(drawer_output,this->image_data_,improc::ImageFileDrawer::kImageNorm) == 0;
    }
};
