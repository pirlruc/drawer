#include <improc/drawer/drawer_types/data_matrix_drawer.hpp>

/**
 * @brief Construct a new improc::DataMatrixDrawer object
 */
improc::DataMatrixDrawer::DataMatrixDrawer(): improc::BaseDrawer() 
                                            , writer_(ZXing::DataMatrix::Writer()) 
                                            , hints_(ZXing::DecodeHints()) {}

/**
 * @brief Construct a new improc::DataMatrixDrawer object
 * 
 * @param drawer_json - configuration json for data matrix drawer
 */
improc::DataMatrixDrawer::DataMatrixDrawer(const Json::Value& drawer_json) : improc::DataMatrixDrawer() 
{
    this->Load(std::move(drawer_json));
}

/**
 * @brief Load configuration for a improc::DataMatrixDrawer object
 * 
 * @param drawer_json - configuration json for data matrix drawer
 */
improc::DataMatrixDrawer& improc::DataMatrixDrawer::Load(const Json::Value& drawer_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Creating data matrix drawer...");
    this->writer_.setMargin(improc::DataMatrixDrawer::kMargin);
    this->writer_.setShapeHint(improc::DataMatrixDrawer::kSquareSymbolShape);
    return (*this);
};

/**
 * @brief Draw data matrix
 * 
 * @param message - message to be encoded in data matrix
 * @return cv::Mat - data matrix image with encoded message
 */
cv::Mat improc::DataMatrixDrawer::Draw(const std::optional<improc::DrawerVariant>& message)
{
    IMPROC_DRAWER_LOGGER_TRACE("Drawing data matrix...");
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter {};
    ZXing::BitMatrix matrix_data = this->writer_.encode ( converter.from_bytes(std::get<std::string>(message.value()))
                                                        , improc::DataMatrixDrawer::kMinWidth
                                                        , improc::DataMatrixDrawer::kMinHeight );
    cv::Mat data_matrix (matrix_data.height(),matrix_data.width(),improc::BaseDrawer::kImageDataType);
    std::transform  ( matrix_data.row(0).begin()
                    , matrix_data.row(matrix_data.height()).end()
                    , data_matrix.begin<uint8_t>()
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
    return data_matrix;
};

/**
 * @brief Verify message encoded in data matrix
 * 
 * @param drawer_output - data matrix image
 * @param message - message encoded in data matrix
 * @return bool - true if message and message recovered from the data matrix image is the same, false otherwise.
 */
bool improc::DataMatrixDrawer::Verify(const cv::Mat& drawer_output, const std::optional<improc::DrawerVariant>& message)
{
    IMPROC_DRAWER_LOGGER_TRACE("Verifying data matrix content...");
    std::unique_ptr<ZXing::BinaryBitmap> data_matrix_bitmap = 
        std::make_unique<ZXing::ThresholdBinarizer> ( ZXing::ImageView  ( drawer_output.data
                                                                        , drawer_output.cols
                                                                        , drawer_output.rows
                                                                        , improc::DataMatrixDrawer::kImageFormat
                                                    ), 0 );
    ZXing::DecoderResult result = ZXing::DataMatrix::Decode(*(data_matrix_bitmap->getBitMatrix()),this->hints_.characterSet());
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