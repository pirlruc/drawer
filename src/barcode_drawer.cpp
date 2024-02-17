#include <improc/drawer/drawer_types/barcode_drawer.hpp>

/**
 * @brief Construct a new improc::BarcodeDrawer object
 */
improc::BarcodeDrawer::BarcodeDrawer()  : improc::BaseDrawer() 
                                        , writer_(ZXing::OneD::Code128Writer())
                                        , reader_options_(ZXing::ReaderOptions())
{
    IMPROC_DRAWER_LOGGER_TRACE("Creating default barcode drawer...");
    this->reader_options_.setTryHarder(improc::BarcodeDrawer::kTryHarder);
    this->reader_options_.setTryRotate(improc::BarcodeDrawer::kDoNotRotate);
    this->reader_options_.setIsPure(improc::BarcodeDrawer::kNotPure);
    this->reader_options_.setMinLineCount(improc::BarcodeDrawer::kMinHeight);
    this->reader_options_.setFormats(improc::BarcodeDrawer::kBarcodeFormat);
}

/**
 * @brief Construct a new improc::BarcodeDrawer object
 * 
 * @param drawer_json - configuration json for barcode drawer
 */
improc::BarcodeDrawer::BarcodeDrawer(const Json::Value& drawer_json) : improc::BarcodeDrawer() 
{
    this->Load(std::move(drawer_json));
}

/**
 * @brief Load configuration for a improc::BarcodeDrawer object
 * 
 * @param drawer_json - configuration json for barcode drawer
 */
improc::BarcodeDrawer& improc::BarcodeDrawer::Load(const Json::Value& drawer_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Creating barcode drawer...");
    this->writer_.setMargin(improc::BarcodeDrawer::kMargin);
    return (*this);
};

/**
 * @brief Draw barcode
 * 
 * @param message - message to be encoded in barcode
 * @return cv::Mat - barcode image with encoded message
 */
cv::Mat improc::BarcodeDrawer::Draw(const std::optional<improc::DrawerVariant>& message)
{
    IMPROC_DRAWER_LOGGER_TRACE("Drawing barcode...");
    ZXing::BitMatrix barcode_data = this->writer_.encode( std::get<std::string>(message.value())
                                                        , improc::BarcodeDrawer::kMinWidth
                                                        , improc::BarcodeDrawer::kMinHeight );
    cv::Mat barcode ( barcode_data.height(),barcode_data.width(),improc::BaseDrawer::kImageDataType );
    std::transform  ( barcode_data.row(0).begin()
                    , barcode_data.row(barcode_data.height()).end()
                    , barcode.begin<uint8_t>()
                    , [] (const uint8_t& bitmatrix_item) 
                        {
                            if (bitmatrix_item != 0)
                            {
                                return improc::BaseDrawer::kBlackValue;
                            }
                            else
                            {
                                return improc::BaseDrawer::kWhiteValue;
                            }
                        }
                    );
    return barcode;
};

/**
 * @brief Verify message encoded in barcode
 * 
 * @param drawer_output - barcode image
 * @param message - message encoded in barcode
 * @return bool - true if message and message recovered from the barcode image is the same, false otherwise.
 */
bool improc::BarcodeDrawer::Verify(const cv::Mat& drawer_output, const std::optional<improc::DrawerVariant>& message)
{
    IMPROC_DRAWER_LOGGER_TRACE("Verifying barcode content...");
    ZXing::OneD::Reader reader {this->reader_options_};
    std::unique_ptr<ZXing::BinaryBitmap> barcode_bitmap = 
        std::make_unique<ZXing::ThresholdBinarizer> ( ZXing::ImageView  ( drawer_output.data
                                                                        , drawer_output.cols
                                                                        , drawer_output.rows
                                                                        , improc::BarcodeDrawer::kImageFormat
                                                    ), 0 );
    ZXing::Result result = reader.decode(*barcode_bitmap);
    if (result.isValid() == true)
    {
        return result.text() == std::get<std::string>(message.value());
    }
    else
    {
        return false;
    }
}