#include <improc/drawer/engine/element_drawer.hpp>

improc::ElementDrawer::ElementDrawer()  : drawer_(std::shared_ptr<improc::BaseDrawer>())
                                        , rotation_(std::optional<improc::RotationType>()) 
                                        , size_(std::optional<cv::Size>()) {};

improc::ElementDrawer::ElementDrawer(const improc::DrawerFactory& factory, const Json::Value& element_drawer_json) : improc::ElementDrawer()
{
    this->Load(factory,element_drawer_json);
}

improc::ElementDrawer& improc::ElementDrawer::Load(const improc::DrawerFactory& factory, const Json::Value& element_drawer_json) 
{
    IMPROC_DRAWER_LOGGER_TRACE("Creating element drawer...");
    static const std::string kRotationKey = "rotation";
    static const std::string kElemSizeKey = "drawer-size";
    if (element_drawer_json.isMember(kRotationKey) == true)
    {
        this->rotation_ = improc::RotationType(improc::json::ReadElement<std::string>(element_drawer_json[kRotationKey]));
    }
    if (element_drawer_json.isMember(kElemSizeKey) == true)
    {
        this->size_     = improc::json::ReadPositiveSize<cv::Size>(element_drawer_json[kElemSizeKey]);
    }
    this->drawer_ = improc::BaseDrawer::Create(factory,element_drawer_json);
    return (*this);
}

unsigned int improc::ElementDrawer::GetScale(const cv::Size& current_size, const cv::Size& expected_size)
{
    IMPROC_DRAWER_LOGGER_TRACE("Obtaining scale factor...");
    double scaling_width  = static_cast<double>(expected_size.width)  / static_cast<double>(current_size.width);
    double scaling_height = static_cast<double>(expected_size.height) / static_cast<double>(current_size.height);
    unsigned int scaling  = std::min(std::round(scaling_width),std::round(scaling_height)); 
    if (scaling <= 1)
    {
        std::string error_message = fmt::format("Scale should be greater than 1 for element drawer. Scale obtained was {}",scaling);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::value_error(std::move(error_message));
    }
    return scaling;
}

cv::Mat improc::ElementDrawer::Draw(const std::optional<std::string>& message) const
{
    IMPROC_DRAWER_LOGGER_TRACE("Drawing element...");
    if (this->drawer_ == nullptr)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: Drawer is not defined.");
        throw improc::drawer_not_defined();
    }

    cv::Mat drawer_output = this->drawer_->Draw(message);
    if (this->rotation_.has_value() == true)
    {
        drawer_output = this->rotation_.value().Apply(drawer_output);
    }
    if (this->size_.has_value() == true)
    {
        unsigned int scale = improc::ElementDrawer::GetScale(drawer_output.size(),this->size_.value());
        cv::resize(drawer_output,drawer_output,cv::Size(),scale,scale,cv::INTER_NEAREST);
    }
    IMPROC_DRAWER_LOGGER_DEBUG("Drawing element with size width = {}, height = {}", drawer_output.cols, drawer_output.rows);
    return drawer_output;
}

bool improc::ElementDrawer::Verify(const cv::Mat& drawer_output, const std::optional<std::string>& message) const
{
    IMPROC_DRAWER_LOGGER_TRACE("Verifying element...");
    if (this->drawer_ == nullptr)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: Drawer is not defined.");
        throw improc::drawer_not_defined();
    }

    cv::Mat drawer_transformed = drawer_output.clone();
    if (this->size_.has_value() == true)
    {
        unsigned int scale = 0;
        if (this->rotation_.has_value() == true)
        {
            scale = improc::ElementDrawer::GetScale ( this->rotation_.value().Apply(this->drawer_->Draw(message)).size()
                                                    , this->size_.value() );
        }
        else
        {
            scale = improc::ElementDrawer::GetScale(this->drawer_->Draw(message).size(),this->size_.value());
        }
        cv::resize(drawer_transformed,drawer_transformed,cv::Size(),1.0/scale,1.0/scale,cv::INTER_NEAREST);
    }
    if (this->rotation_.has_value() == true)
    {
        drawer_transformed = this->rotation_.value().ApplyInverse(drawer_transformed);
    }
    IMPROC_DRAWER_LOGGER_DEBUG("Verifying element with size width = {}, height = {}", drawer_transformed.cols, drawer_transformed.rows);
    return this->drawer_->Verify(drawer_transformed,message);
}
