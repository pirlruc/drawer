        // TODO: Prepare point and size parser to allow other types beside int. See metric_pixel_json_converter.hpp
        template<>
        inline cv::Point improc::json::ReadElement(const Json::Value& json_elem)
        {
            IMPROC_DRAWER_LOGGER_TRACE("Reading cv::Point json element...");
            static const std::string kXPositionKey = "x";
            static const std::string kYPositionKey = "y";
            if (json_elem.isMember(kXPositionKey) == false)
            {
                IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: x-position missing.");
                throw improc::file_processing_error();
            }
            if (json_elem.isMember(kYPositionKey) == false)
            {
                IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: y-position missing.");
                throw improc::file_processing_error();
            }
            return cv::Point( improc::json::ReadElement<int>(json_elem[kXPositionKey])
                            , improc::json::ReadElement<int>(json_elem[kYPositionKey]) );
        }

        template<>
        inline cv::Size improc::json::ReadElement(const Json::Value& json_elem)
        {
            IMPROC_DRAWER_LOGGER_TRACE("Reading cv::Size json element...");
            static const std::string kWidthKey  = "width";
            static const std::string kHeightKey = "height";
            if (json_elem.isMember(kWidthKey) == false)
            {
                IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: Width missing.");
                throw improc::file_processing_error();
            }
            if (json_elem.isMember(kHeightKey) == false)
            {
                IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: Height missing.");
                throw improc::file_processing_error();
            }
            return cv::Size( improc::json::ReadElement<int>(json_elem[kWidthKey])
                           , improc::json::ReadElement<int>(json_elem[kHeightKey]) );
        }
