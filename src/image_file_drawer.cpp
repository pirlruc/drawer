#include <improc/drawer/drawer_types/image_file_drawer.hpp>

improc::ImageFileDrawer::ImageFileDrawer()  : improc::BaseDrawer()
                                            , image_data_(cv::Mat()) {};

improc::ImageFileDrawer::ImageFileDrawer(const Json::Value& drawer_json) : improc::ImageFileDrawer() 
{
    this->Load(std::move(drawer_json));
};

improc::ImageFileDrawer& improc::ImageFileDrawer::Load(const Json::Value& drawer_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Creating image file drawer...");
    static const std::string kImageFileKey = "image-filepath";
    if (drawer_json.isMember(kImageFileKey) == false) 
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: Image filepath missing.");
        throw improc::file_processing_error();
    }
    improc::File image_file {drawer_json[kImageFileKey],improc::ApplicationContext::get()->get_application_folder()};
    this->image_data_ = cv::imread(image_file.get_filepath(),improc::ImageFileDrawer::kImageReadMode);
    return (*this);
};

cv::Mat improc::ImageFileDrawer::Draw(const std::optional<std::string>& message)
{
    IMPROC_DRAWER_LOGGER_TRACE("Drawing image file content...");
    return this->image_data_;
};
