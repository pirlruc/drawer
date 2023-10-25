#include <improc/drawer/drawer_types/qrcode_drawer.hpp>

/**
 * @brief Construct a new improc::ErrorCorrectionLevel object
 */
improc::qrcode::ErrorCorrectionLevel::ErrorCorrectionLevel(): value_(improc::qrcode::ErrorCorrectionLevel::kLow) {};

/**
 * @brief Construct a new improc::ErrorCorrectionLevel object
 * 
 * @param error_correction_level_str - error correction level description as string
 */
improc::qrcode::ErrorCorrectionLevel::ErrorCorrectionLevel(const std::string& error_correction_level_str)
{
    IMPROC_DRAWER_LOGGER_TRACE("Obtaining error correction level from string {}...",error_correction_level_str);
    static const std::unordered_map<std::string,ErrorCorrectionLevel::Value> kToElemType =  { {"low"     ,ErrorCorrectionLevel::Value::kLow     }
                                                                                            , {"medium"  ,ErrorCorrectionLevel::Value::kMedium  }
                                                                                            , {"quartile",ErrorCorrectionLevel::Value::kQuartile}
                                                                                            , {"high"    ,ErrorCorrectionLevel::Value::kHigh    }
                                                                                            };
    this->value_ = kToElemType.at(improc::String::ToLower(std::move(error_correction_level_str)));
}

/**
 * @brief Construct a new improc::QrCodeDrawer object
 */
improc::QrCodeDrawer::QrCodeDrawer(): improc::BaseDrawer()
                                    , error_correction_level_(improc::qrcode::ErrorCorrectionLevel()) {};

/**
 * @brief Construct a new improc::QrCodeDrawer object
 * 
 * @param drawer_json - configuration json for qr-code drawer
 */
improc::QrCodeDrawer::QrCodeDrawer(const Json::Value& drawer_json) : improc::QrCodeDrawer() 
{
    this->Load(std::move(drawer_json));
}

/**
 * @brief Load configuration for a improc::QrCodeDrawer object
 * 
 * @param drawer_json - configuration json for qr-code drawer
 */
improc::QrCodeDrawer& improc::QrCodeDrawer::Load(const Json::Value& drawer_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Creating qr-code drawer...");
    static const std::string kErrorCorrectionKey = "error-correction-level";
    if (drawer_json.isMember(kErrorCorrectionKey) == false) 
    {
        std::string error_message = fmt::format("Key {} is missing from qr-code drawer json",kErrorCorrectionKey);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::json_error(std::move(error_message));
    }
    this->error_correction_level_ = improc::qrcode::ErrorCorrectionLevel(improc::json::ReadElement<std::string>(drawer_json[kErrorCorrectionKey]));
    return (*this);
};

/**
 * @brief Draw QR-Code
 * 
 * @param message - message to be encoded in qr-code
 * @return cv::Mat - qr-code image with encoded message
 */
cv::Mat improc::QrCodeDrawer::Draw(const std::optional<improc::DrawerVariant>& message)
{
    IMPROC_DRAWER_LOGGER_TRACE("Drawing qrcode...");
    qrcodegen::QrCode qrcode_data = qrcodegen::QrCode::encodeText(std::get<std::string>(message.value()).c_str(),this->error_correction_level_.ToQrCodeGen());
    int qrcode_size = qrcode_data.getSize();
    cv::Mat qrcode (qrcode_size,qrcode_size,improc::BaseDrawer::kImageDataType,improc::BaseDrawer::kWhiteValue);
    for (size_t pixel_y = 0; pixel_y < qrcode_size; pixel_y++)
    {
        auto qrcode_row_ptr = qrcode.ptr<uint8_t>(pixel_y);
        for (size_t pixel_x = 0; pixel_x < qrcode_size; pixel_x++)
        {
            if (qrcode_data.getModule(pixel_x,pixel_y) == true)
            {
                qrcode_row_ptr[pixel_x] = improc::BaseDrawer::kBlackValue;
            }
        }
    }
    return qrcode;
};

/**
 * @brief Verify message encoded in qr-code
 * 
 * @param drawer_output - qr-code image
 * @param message - message encoded in qr-code
 * @return bool - true if message and message recovered from the qr-code image is the same, false otherwise.
 */
bool improc::QrCodeDrawer::Verify(const cv::Mat& drawer_output, const std::optional<improc::DrawerVariant>& message)
{
    IMPROC_DRAWER_LOGGER_TRACE("Verifying qr-code content...");
    std::unique_ptr<ZXing::BinaryBitmap> data_matrix_bitmap = 
        std::make_unique<ZXing::ThresholdBinarizer> ( ZXing::ImageView  ( drawer_output.data
                                                                        , drawer_output.cols
                                                                        , drawer_output.rows
                                                                        , improc::QrCodeDrawer::kImageFormat
                                                    ), 0 );
    ZXing::DecoderResult result = ZXing::QRCode::Decode(*(data_matrix_bitmap->getBitMatrix()));
    if (result.isValid() == true)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter {};
        return converter.to_bytes(result.text()) == std::get<std::string>(message.value());
    }
    else
    {
        return false;
    }
}