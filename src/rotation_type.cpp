#include <improc/drawer/parsers/rotation_type.hpp>

improc::RotationType::RotationType() : value_(improc::RotationType::k0Deg) {};

improc::RotationType::RotationType(const std::string& rotation_type_str)
{
    IMPROC_DRAWER_LOGGER_TRACE("Obtaining rotation type from string {}...",rotation_type_str);
    static const std::unordered_map<std::string,RotationType::Value> kToElemType = { {"0-deg"  ,RotationType::Value::k0Deg  }
                                                                                   , {"90-deg" ,RotationType::Value::k90Deg }
                                                                                   , {"180-deg",RotationType::Value::k180Deg}
                                                                                   , {"270-deg",RotationType::Value::k270Deg}
                                                                                   };
    this->value_ = kToElemType.at(improc::String::ToLower(rotation_type_str));
}

cv::Mat improc::RotationType::Apply(const cv::Mat& image) const
{
    IMPROC_DRAWER_LOGGER_TRACE("Applying rotation...");
    cv::Mat rotated_image {};
    if (this->value_ == improc::RotationType::Value::k90Deg)
    {
        cv::transpose(image,rotated_image);
        cv::flip(rotated_image,rotated_image,+1); // Flip around y-axis
    }
    else if (this->value_ == improc::RotationType::Value::k270Deg)
    {
        cv::transpose(image,rotated_image);
        cv::flip(rotated_image,rotated_image,0); // Flip around x-axis
    }
    else if (this->value_ == improc::RotationType::Value::k180Deg)
    {
        cv::flip(image,rotated_image,-1); // Flip around x- and y-axis
    }
    else
    {
        rotated_image = image;
    }
    return rotated_image;
}

cv::Mat improc::RotationType::ApplyInverse(const cv::Mat& rotated_image) const
{
    IMPROC_DRAWER_LOGGER_TRACE("Applying inverse rotation...");
    cv::Mat image {};
    if (this->value_ == improc::RotationType::Value::k90Deg)
    {
        cv::transpose(rotated_image,image);
        cv::flip(image,image,0); // Flip around x-axis
    }
    else if (this->value_ == improc::RotationType::Value::k270Deg)
    {
        cv::transpose(rotated_image,image);
        cv::flip(image,image,+1); // Flip around y-axis
    }
    else if (this->value_ == improc::RotationType::Value::k180Deg)
    {
        cv::flip(rotated_image,image,-1); // Flip around x- and y-axis
    }
    else
    {
        image = rotated_image;
    }
    return image;
}
