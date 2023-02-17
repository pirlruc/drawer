#include <improc/drawer/drawer_types/text_drawer.hpp>

improc::TextDrawer::TextDrawer(): improc::BaseDrawer()
                                , font_loaded_(false)
                                , image_text_size_(cv::Size())
                                , printing_resolution_(0)
                                , font_size_(0)
{
    IMPROC_DRAWER_LOGGER_TRACE("Initializing FreeType library...");
    FT_Error error = FT_Init_FreeType(&this->library_);
    if (error != FT_Err_Ok)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: Error initializing FreeType library with code {}.",error);
        throw improc::freetype_lib_error();
    }
};

improc::TextDrawer::TextDrawer(const Json::Value& drawer_json) : improc::TextDrawer() 
{
    this->Load(drawer_json);
}

improc::TextDrawer& improc::TextDrawer::Load(const Json::Value& drawer_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Creating text drawer...");
    static const std::string kPrintingResolutionKey = "printing-resolution-dpi";
    static const std::string kImageTextSizeKey      = "image-text-size";
    static const std::string kFontPathKey           = "font-filepath";
    static const std::string kFontSizeKey           = "font-size";
    if (drawer_json.isMember(kPrintingResolutionKey) == false) 
    {
        std::string error_message = fmt::format("Key {} is missing from text drawer json",kPrintingResolutionKey);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::json_error(std::move(error_message));
    }
    if (drawer_json.isMember(kImageTextSizeKey) == false) 
    {
        std::string error_message = fmt::format("Key {} is missing from text drawer json",kImageTextSizeKey);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: " + error_message);
        throw improc::json_error(std::move(error_message));
    }
    if (drawer_json.isMember(kFontPathKey) == false) 
    {
        std::string error_message = fmt::format("Key {} is missing from text drawer json",kFontPathKey);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_03: " + error_message);
        throw improc::json_error(std::move(error_message));
    }
    if (drawer_json.isMember(kFontSizeKey) == false) 
    {
        std::string error_message = fmt::format("Key {} is missing from text drawer json",kFontSizeKey);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_04: " + error_message);
        throw improc::json_error(std::move(error_message));
    }
    this->printing_resolution_ = improc::json::ReadElement<unsigned int>(drawer_json[kPrintingResolutionKey]);
    this->image_text_size_     = improc::TextDrawer::ParseMetricSize(drawer_json[kImageTextSizeKey],improc::MetricPixelConverter(this->printing_resolution_));
    
    improc::File font_file {drawer_json[kFontPathKey],improc::ApplicationContext::get()->get_application_folder()};
    FT_Error error_font = FT_New_Face(this->library_,font_file.get_filepath().c_str(),improc::TextDrawer::kLoadFirstFont,&this->face_);
    if (error_font != FT_Err_Ok)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_07: Error loading face font from {} with code {}.",font_file.get_filepath(),error_font);
        throw improc::freetype_lib_error();
    }
    this->font_loaded_  = true;

    unsigned int font_size = improc::json::ReadElement<unsigned int>(drawer_json[kFontSizeKey]);
    if (font_size == 0)
    {
        std::string error_message = fmt::format("Font size should be greater than zero. {} font size was gave",font_size);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_08: " + error_message);
        throw improc::value_error(std::move(error_message));
    }
    this->font_size_ = font_size;

    FT_Error error_char_size = FT_Set_Char_Size ( this->face_
                                                , this->font_size_ * improc::TextDrawer::kFontSizePointFraction
                                                , this->font_size_ * improc::TextDrawer::kFontSizePointFraction
                                                , this->printing_resolution_, this->printing_resolution_ );
    if (error_char_size != FT_Err_Ok)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_10: Error defining face font size with code {}",error_char_size);
        throw improc::freetype_lib_error();
    }
    return (*this);
};

cv::Size improc::TextDrawer::ParseMetricSize(const Json::Value& size_json, const improc::MetricPixelConverter& pixel_converter)
{
    IMPROC_DRAWER_LOGGER_TRACE("Parsing metric size...");
    cv::Size image_text_size = improc::MetricPixelJsonConverter::MetricSize2PixelSize(size_json,pixel_converter);
    if (image_text_size.width <= 0)
    {
        std::string error_message = fmt::format("Width should be greater than zero. {} width was gave",image_text_size.width);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::value_error(std::move(error_message));
    }
    if (image_text_size.height <= 0)
    {
        std::string error_message = fmt::format("Height should be greater than zero. {} height was gave",image_text_size.height);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: " + error_message);
        throw improc::value_error(std::move(error_message));
    }
    return image_text_size;
}

cv::Mat improc::TextDrawer::Draw(const std::optional<std::string>& message)
{
    IMPROC_DRAWER_LOGGER_TRACE("Drawing text...");
    if (this->font_loaded_ == false)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: Font not loaded.");
        throw improc::freetype_lib_error();
    }

    cv::Mat text_image = 255 * cv::Mat::ones(this->image_text_size_,improc::TextDrawer::kImageDataType);
    IMPROC_DRAWER_LOGGER_DEBUG("Text image with size width = {}, height = {}", text_image.cols, text_image.rows);
    std::string message_data = message.value();
    FT_Pos position_x = 0;
    //TODO: Use threads to improve performance of for loop
    for (size_t char_idx = 0; char_idx < message_data.size(); char_idx++)
    {
        FT_Error error = FT_Load_Char(this->face_,message_data[char_idx],FT_LOAD_RENDER);
        if(error != FT_Err_Ok)
        {
            IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: Error rendering character {} with code {}",message_data[char_idx],error);
            throw improc::freetype_lib_error();
        }

        cv::Mat char_image = this->Bitmap2Mat(this->face_->glyph->bitmap);
        IMPROC_DRAWER_LOGGER_DEBUG  ( "Drawing char in x = {}, y = {} with size width = {}, height = {}"
                                    , position_x, this->image_text_size_.height - char_image.rows, char_image.cols, char_image.rows);
        if (char_image.empty() == false)
        {
            char_image.copyTo(text_image(cv::Rect(position_x,this->image_text_size_.height - char_image.rows,char_image.cols,char_image.rows)));
        }
        position_x += this->face_->glyph->advance.x / static_cast<signed long>(improc::TextDrawer::kFontSizePointFraction);
    }
    return text_image;
};

cv::Mat improc::TextDrawer::Bitmap2Mat(const FT_Bitmap& char_bitmap) const
{
    IMPROC_DRAWER_LOGGER_TRACE("Drawing character on text image...");
    cv::Mat char_image = 255 * cv::Mat::ones(char_bitmap.rows,char_bitmap.width,improc::TextDrawer::kImageDataType);
    //TODO: Use threads to improve performance of for loop
    for (size_t pixel_x = 0; pixel_x < char_bitmap.width; pixel_x++)
    {
        for (size_t pixel_y = 0; pixel_y < char_bitmap.rows; pixel_y++)
        {
            char_image.at<uint8_t>(cv::Point(pixel_x,pixel_y)) = 255 - char_bitmap.buffer[pixel_y * char_bitmap.width + pixel_x];
        }
    }
    return char_image;
}