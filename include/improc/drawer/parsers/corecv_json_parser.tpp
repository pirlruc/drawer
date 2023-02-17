// TODO: Prepare point and size parser to allow other types beside int
template<>
inline cv::Point improc::json::ReadElement(const Json::Value& json_elem)
{
    IMPROC_DRAWER_LOGGER_TRACE("Reading cv::Point json element...");
    static const std::string kXPositionKey = "x";
    static const std::string kYPositionKey = "y";
    if (json_elem.isMember(kXPositionKey) == false)
    {
        std::string error_message = fmt::format("Key {}-position is missing from json",kXPositionKey);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::json_error(std::move(error_message));
    }
    if (json_elem.isMember(kYPositionKey) == false)
    {
        std::string error_message = fmt::format("Key {}-position is missing from json",kYPositionKey);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: " + error_message);
        throw improc::json_error(std::move(error_message));
    }
    return cv::Point( improc::json::ReadElement<int>(json_elem[kXPositionKey])
                    , improc::json::ReadElement<int>(json_elem[kYPositionKey]) );
}

template<>
inline cv::Point2d improc::json::ReadElement(const Json::Value& json_elem)
{
    IMPROC_DRAWER_LOGGER_TRACE("Reading cv::Point json element...");
    static const std::string kXPositionKey = "x";
    static const std::string kYPositionKey = "y";
    if (json_elem.isMember(kXPositionKey) == false)
    {
        std::string error_message = fmt::format("Key {}-position is missing from json",kXPositionKey);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::json_error(std::move(error_message));
    }
    if (json_elem.isMember(kYPositionKey) == false)
    {
        std::string error_message = fmt::format("Key {}-position is missing from json",kYPositionKey);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: " + error_message);
        throw improc::json_error(std::move(error_message));
    }
    return cv::Point2d( improc::json::ReadElement<double>(json_elem[kXPositionKey])
                      , improc::json::ReadElement<double>(json_elem[kYPositionKey]) );
}

template<>
inline cv::Size improc::json::ReadElement(const Json::Value& json_elem)
{
    IMPROC_DRAWER_LOGGER_TRACE("Reading cv::Size json element...");
    static const std::string kWidthKey  = "width";
    static const std::string kHeightKey = "height";
    if (json_elem.isMember(kWidthKey) == false)
    {
        std::string error_message = fmt::format("Key {} is missing from json",kWidthKey);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::json_error(std::move(error_message));
    }
    if (json_elem.isMember(kHeightKey) == false)
    {
        std::string error_message = fmt::format("Key {} is missing from json",kHeightKey);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: " + error_message);
        throw improc::json_error(std::move(error_message));
    }
    return cv::Size ( improc::json::ReadElement<int>(json_elem[kWidthKey])
                    , improc::json::ReadElement<int>(json_elem[kHeightKey]) );
}

template<>
inline cv::Size2d improc::json::ReadElement(const Json::Value& json_elem)
{
    IMPROC_DRAWER_LOGGER_TRACE("Reading cv::Size json element...");
    static const std::string kWidthKey  = "width";
    static const std::string kHeightKey = "height";
    if (json_elem.isMember(kWidthKey) == false)
    {
        std::string error_message = fmt::format("Key {} is missing from json",kWidthKey);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::json_error(std::move(error_message));
    }
    if (json_elem.isMember(kHeightKey) == false)
    {
        std::string error_message = fmt::format("Key {} is missing from json",kHeightKey);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: " + error_message);
        throw improc::json_error(std::move(error_message));
    }
    return cv::Size2d( improc::json::ReadElement<double>(json_elem[kWidthKey])
                     , improc::json::ReadElement<double>(json_elem[kHeightKey]) );
}

template<typename KeyType>
inline KeyType improc::json::ReadPositiveSize(const Json::Value& json_size)
{
    IMPROC_DRAWER_LOGGER_TRACE("Reading positive {} json element...",typeid(KeyType).name());
    KeyType size = improc::json::ReadElement<KeyType>(json_size);
    if (size.width <= 0.0)
    {
        std::string error_message = fmt::format("Width should be greater than zero. Width gave was {}",size.width);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::value_error(std::move(error_message));
    }
    if (size.height <= 0.0)
    {
        std::string error_message = fmt::format("Height should be greater than zero. Height gave was {}",size.height);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: " + error_message);
        throw improc::value_error(std::move(error_message));
    }
    return size;
}
