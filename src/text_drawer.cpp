#include <improc/drawer/drawer_types/text_drawer.hpp>

improc::TextRenderMode::TextRenderMode() : value_(improc::TextRenderMode::kGrayscale) {};

improc::TextRenderMode::TextRenderMode(const std::string& text_render_mode_str)
{
    IMPROC_DRAWER_LOGGER_TRACE("Obtaining text render mode from string {}...",text_render_mode_str);
    static const std::unordered_map<std::string,TextRenderMode::Value> kToElemType =    { {"binary"     ,TextRenderMode::Value::kBinary     }
                                                                                        , {"grayscale"  ,TextRenderMode::Value::kGrayscale  }
                                                                                        };
    this->value_ = kToElemType.at(improc::String::ToLower(text_render_mode_str));
}

improc::TextDrawer::TextDrawer(): improc::BaseDrawer()
                                , font_loaded_(false)
                                , image_text_size_(cv::Size())
                                , printing_resolution_(0)
                                , font_size_(0)
                                , font_spacing_(0)
                                , render_mode_(improc::TextRenderMode())
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
    static const std::string kFontSpacingKey        = "font-spacing";
    static const std::string kRenderModeKey         = "render-mode";    
    if (drawer_json.isMember(kPrintingResolutionKey) == false) 
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: Printing resolution missing.");
        throw improc::file_processing_error();
    }
    if (drawer_json.isMember(kImageTextSizeKey) == false) 
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: Image text size missing.");
        throw improc::file_processing_error();
    }
    if (drawer_json.isMember(kFontPathKey) == false) 
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_03: Font filepath missing.");
        throw improc::file_processing_error();
    }
    if (drawer_json.isMember(kFontSizeKey) == false) 
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_04: Font size missing.");
        throw improc::file_processing_error();
    }
    if (drawer_json.isMember(kFontSpacingKey) == false) 
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_05: Font spacing missing.");
        throw improc::file_processing_error();
    }
    if (drawer_json.isMember(kRenderModeKey) == false) 
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_06: Text render mode missing.");
        throw improc::file_processing_error();
    }
    this->printing_resolution_ = improc::json::ReadElement<unsigned int>(drawer_json[kPrintingResolutionKey]);
    this->image_text_size_     = improc::TextDrawer::ParseMetricSize(drawer_json[kImageTextSizeKey],improc::MetricPixelConverter(this->printing_resolution_));
    
    // TODO: Move filepath parsing to a file parsing structure
    std::filesystem::path font_filepath = improc::ApplicationContext::get()->get_application_folder();
    std::vector<std::string> json_file  = improc::json::ReadVector<std::string>(drawer_json[kFontPathKey]);
    std::for_each(json_file.begin(),json_file.end(), [&font_filepath] (const std::string& directory) {font_filepath /= directory;});
    improc::File font_file {std::move(font_filepath)};
    FT_Error error_font = FT_New_Face(this->library_,font_file.get_filepath().c_str(),improc::TextDrawer::kLoadFirstFont,&this->face_);
    if (error_font != FT_Err_Ok)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_07: Error loading face font from {} with code {}.",font_file.get_filepath(),error_font);
        throw improc::freetype_lib_error();
    }
    this->font_loaded_  = true;

    unsigned int font_size = improc::json::ReadElement<unsigned int>(drawer_json[kFontSizeKey]);
    if (font_size <= 0)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_08: Font size should be greater than zero");
        throw improc::file_processing_error();
    }
    this->font_size_ = font_size;

    unsigned int font_spacing = improc::json::ReadElement<unsigned int>(drawer_json[kFontSpacingKey]);
    if (font_spacing <= 0)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_09: Font size should be greater than zero");
        throw improc::file_processing_error();
    }
    this->font_spacing_ = font_spacing;
    this->render_mode_  = improc::json::ReadElement<std::string>(drawer_json[kRenderModeKey]);
    
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
    static const std::string kMetricUnitKey         = "metric-unit";
    if (size_json.isMember(kMetricUnitKey) == false)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: Metric units for metric size missing.");
        throw improc::file_processing_error();
    }
    improc::MetricUnit metric = improc::json::ReadElement<std::string>(size_json[kMetricUnitKey]);
    cv::Size2d text_size      = improc::MetricPixelJsonConverter::ParseSize(size_json);
    cv::Size image_text_size  = cv::Size(pixel_converter.Metric2Pixel(text_size.width,metric),pixel_converter.Metric2Pixel(text_size.height,metric));
    if (image_text_size.width <= 0)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_02: Width should be greater than zero");
        throw improc::file_processing_error();
    }
    if (image_text_size.height <= 0)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_03: Height should be greater than zero");
        throw improc::file_processing_error();
    }
    return image_text_size;
}

cv::Mat improc::TextDrawer::Draw(const std::optional<std::string>& message) const
{
    IMPROC_DRAWER_LOGGER_TRACE("Drawing text...");
    if (this->font_loaded_ == false)
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: Font not loaded.");
        throw improc::freetype_lib_error();
    }

    cv::Mat text_image = 255 * cv::Mat::ones(this->image_text_size_,improc::TextDrawer::kImageDataType);
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
        char_image.copyTo(text_image(cv::Rect(position_x,this->image_text_size_.height - char_image.rows,char_image.cols,char_image.rows)));
        position_x += this->face_->glyph->advance.x >> this->font_spacing_;
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