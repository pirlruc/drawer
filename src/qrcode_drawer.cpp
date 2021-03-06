#include <improc/drawer/drawer_types/qrcode_drawer.hpp>

improc::qrcode::ErrorCorrectionLevel::ErrorCorrectionLevel(): value_(improc::qrcode::ErrorCorrectionLevel::kLow) {};

improc::qrcode::ErrorCorrectionLevel::ErrorCorrectionLevel(const std::string& error_correction_level_str)
{
    IMPROC_DRAWER_LOGGER_TRACE("Obtaining error correction level from string {}...",error_correction_level_str);
    static const std::unordered_map<std::string,ErrorCorrectionLevel::Value> kToElemType =  { {"low"     ,ErrorCorrectionLevel::Value::kLow     }
                                                                                            , {"medium"  ,ErrorCorrectionLevel::Value::kMedium  }
                                                                                            , {"quartile",ErrorCorrectionLevel::Value::kQuartile}
                                                                                            , {"high"    ,ErrorCorrectionLevel::Value::kHigh    }
                                                                                            };
    this->value_ = kToElemType.at(improc::String::ToLower(error_correction_level_str));
}

improc::QrCodeDrawer::QrCodeDrawer(): improc::BaseDrawer()
                                    , error_correction_level_(improc::qrcode::ErrorCorrectionLevel()) 
                                    , hints_(ZXing::DecodeHints()) {};

improc::QrCodeDrawer::QrCodeDrawer(const Json::Value& drawer_json) : improc::QrCodeDrawer() 
{
    this->Load(drawer_json);
}

improc::QrCodeDrawer& improc::QrCodeDrawer::Load(const Json::Value& drawer_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Creating qr-code drawer...");
    static const std::string kErrorCorrectionKey = "error-correction-level";
    if (drawer_json.isMember(kErrorCorrectionKey) == false) 
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: Error correction level missing for qr-code.");
        throw improc::file_processing_error();
    }
    this->error_correction_level_ = improc::qrcode::ErrorCorrectionLevel(improc::json::ReadElement<std::string>(drawer_json[kErrorCorrectionKey]));
    return (*this);
};

cv::Mat improc::QrCodeDrawer::Draw(const std::optional<std::string>& message)
{
    IMPROC_DRAWER_LOGGER_TRACE("Drawing qrcode...");
    qrcodegen::QrCode qrcode_data = qrcodegen::QrCode::encodeText(message.value().c_str(),this->error_correction_level_.ToQrCodeGen());
    int qrcode_size = qrcode_data.getSize();
    cv::Mat qrcode = 255 * cv::Mat::ones(qrcode_size,qrcode_size,improc::QrCodeDrawer::kImageDataType);
    // TODO: Improve method to draw qr-code using threads
    for (size_t pixel_x = 0; pixel_x < qrcode_size; pixel_x++)
    {
        for (size_t pixel_y = 0; pixel_y < qrcode_size; pixel_y++)
        {
            if (qrcode_data.getModule(pixel_x,pixel_y) == true)
            {
                qrcode.at<uint8_t>(cv::Point(pixel_x,pixel_y)) = 0;
            }
        }
    }
    return qrcode;
};

bool improc::QrCodeDrawer::Verify(const cv::Mat& drawer_output, const std::optional<std::string>& message)
{
    IMPROC_DRAWER_LOGGER_TRACE("Verifying qr-code content...");
    std::unique_ptr<ZXing::BinaryBitmap> data_matrix_bitmap = 
        std::make_unique<ZXing::ThresholdBinarizer> ( ZXing::ImageView  ( drawer_output.data
                                                                        , drawer_output.cols
                                                                        , drawer_output.rows
                                                                        , improc::QrCodeDrawer::kImageFormat
                                                    ), 0 );
    ZXing::DecoderResult result = ZXing::QRCode::Decode(*(data_matrix_bitmap->getBitMatrix()),this->hints_.characterSet());
    if (result.isValid() == true)
    {
        // TODO: Add functionality to convert strings in string object
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter {};
        return converter.to_bytes(result.text()) == message.value();
    }
    else
    {
        return false;
    }
}