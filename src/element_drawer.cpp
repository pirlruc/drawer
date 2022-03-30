#include <improc/drawer/element_drawer.hpp>

improc::ElementDrawer::ElementDrawer()  : drawer_(std::shared_ptr<improc::BaseDrawer>())
                                        , rotation_(std::optional<improc::RotationType>()) 
                                        , scale_(std::optional<unsigned int>()) {};

improc::ElementDrawer::ElementDrawer(const improc::DrawerFactory& factory, const Json::Value& element_drawer_json)
{
    this->Load(factory,element_drawer_json);
}

improc::ElementDrawer& improc::ElementDrawer::Load(const improc::DrawerFactory& factory, const Json::Value& element_drawer_json) 
{
    static const std::string kRotationKey = "rotation";
    static const std::string kScaleKey    = "scale";
    this->drawer_ = improc::BaseDrawer::Create(factory,element_drawer_json);
    if (element_drawer_json.isMember(kRotationKey) == true)
    {
        this->rotation_ = improc::json::ReadElement<std::string>(element_drawer_json[kRotationKey]);
    }
    if (element_drawer_json.isMember(kScaleKey) == true)
    {
        this->scale_    = improc::ElementDrawer::ParseScale(element_drawer_json[kScaleKey]);
    }
    return (*this);
}

unsigned int improc::ElementDrawer::ParseScale(const Json::Value& scale_json)
{
    unsigned int scale = improc::json::ReadElement<unsigned int>(scale_json);
    if (scale <= 1)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: Scale should be greater than 1.");
        throw improc::file_processing_error();
    }
    return scale;
}

cv::Mat improc::ElementDrawer::Draw() const
{
    if (this->drawer_ == nullptr)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: Drawer is not defined.");
        throw improc::drawer_not_defined();
    }

    cv::Mat drawer_output = this->drawer_->Draw();
    if (this->scale_.has_value() == true)
    {
        unsigned int scale = this->scale_.value();
        cv::resize(drawer_output,drawer_output,cv::Size(),scale,scale,cv::INTER_NEAREST);
    }
    if (this->rotation_.has_value() == true)
    {
        drawer_output = this->rotation_.value().Apply(drawer_output);
    }
    return drawer_output;
}
