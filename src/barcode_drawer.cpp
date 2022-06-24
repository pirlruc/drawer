#include <improc/drawer/drawer_types/barcode_drawer.hpp>

improc::BarcodeDrawer::BarcodeDrawer()  : improc::BaseDrawer() 
                                        , writer_(ZXing::OneD::Code128Writer())
                                        , hints_(ZXing::DecodeHints())
{
    IMPROC_DRAWER_LOGGER_TRACE("Creating default barcode drawer...");
    this->hints_.setTryHarder(improc::BarcodeDrawer::kTryHarder);
    this->hints_.setTryRotate(improc::BarcodeDrawer::kDoNotRotate);
    this->hints_.setIsPure(improc::BarcodeDrawer::kNotPure);
    this->hints_.setMinLineCount(improc::BarcodeDrawer::kMinHeight);
    this->hints_.setFormats(improc::BarcodeDrawer::kBarcodeFormat);
}

improc::BarcodeDrawer::BarcodeDrawer(const Json::Value& drawer_json) : improc::BarcodeDrawer() 
{
    this->Load(drawer_json);
}

improc::BarcodeDrawer& improc::BarcodeDrawer::Load(const Json::Value& drawer_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Creating barcode drawer...");
    this->writer_.setMargin(improc::BarcodeDrawer::kMargin);
    return (*this);
};

cv::Mat improc::BarcodeDrawer::Draw(const std::optional<std::string>& message)
{
    IMPROC_DRAWER_LOGGER_TRACE("Drawing barcode...");
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter {};
    ZXing::BitMatrix barcode_data = this->writer_.encode( converter.from_bytes(message.value())
                                                        , improc::BarcodeDrawer::kMinWidth
                                                        , improc::BarcodeDrawer::kMinHeight );
    cv::Mat barcode = 255 * cv::Mat::ones(barcode_data.height(),barcode_data.width(),improc::BarcodeDrawer::kImageDataType);
    // TODO: Improve method to draw barcode using threads
    for (size_t pixel_x = 0; pixel_x < barcode_data.width(); pixel_x++)
    {
        for (size_t pixel_y = 0; pixel_y < barcode_data.height(); pixel_y++)
        {
            if (barcode_data.get(pixel_x,pixel_y) == true)
            {
                barcode.at<uint8_t>(cv::Point(pixel_x,pixel_y)) = 0;
            }
        }
    }
    return barcode;
};

bool improc::BarcodeDrawer::Verify(const cv::Mat& drawer_output, const std::optional<std::string>& message)
{
    IMPROC_DRAWER_LOGGER_TRACE("Verifying barcode content...");
    ZXing::OneD::Reader reader {this->hints_};
    std::unique_ptr<ZXing::BinaryBitmap> barcode_bitmap = 
        std::make_unique<ZXing::ThresholdBinarizer> ( ZXing::ImageView  ( drawer_output.data
                                                                        , drawer_output.cols
                                                                        , drawer_output.rows
                                                                        , improc::BarcodeDrawer::kImageFormat
                                                    ), 0 );
    ZXing::Result result = reader.decode(*barcode_bitmap);
    if (result.isValid() == true)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter {};
        return converter.to_bytes(result.text()) == message.value();
    }
    else
    {
        return false;
    }
}