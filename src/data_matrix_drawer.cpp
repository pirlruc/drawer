#include <improc/drawer/drawer_types/data_matrix_drawer.hpp>

improc::DataMatrixDrawer::DataMatrixDrawer(): improc::BaseDrawer() 
                                            , writer_(ZXing::DataMatrix::Writer()) 
                                            , hints_(ZXing::DecodeHints()) {}

improc::DataMatrixDrawer::DataMatrixDrawer(const Json::Value& drawer_json) : improc::DataMatrixDrawer() 
{
    this->Load(drawer_json);
}

improc::DataMatrixDrawer& improc::DataMatrixDrawer::Load(const Json::Value& drawer_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Creating data matrix drawer...");
    this->writer_.setMargin(improc::DataMatrixDrawer::kMargin);
    this->writer_.setShapeHint(improc::DataMatrixDrawer::kSquareSymbolShape);
    return (*this);
};

cv::Mat improc::DataMatrixDrawer::Draw(const std::optional<std::string>& message)
{
    IMPROC_DRAWER_LOGGER_TRACE("Drawing data matrix...");
    // TODO: Add functionality to convert strings in string object
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter {};
    ZXing::BitMatrix matrix_data = this->writer_.encode ( converter.from_bytes(message.value())
                                                        , improc::DataMatrixDrawer::kMinWidth
                                                        , improc::DataMatrixDrawer::kMinHeight );
    cv::Mat data_matrix = 255 * cv::Mat::ones(matrix_data.height(),matrix_data.width(),improc::DataMatrixDrawer::kImageDataType);
    // TODO: Improve method to draw barcode using threads
    // TODO: Move image conversion method to a base image class
    for (size_t pixel_x = 0; pixel_x < matrix_data.width(); pixel_x++)
    {
        for (size_t pixel_y = 0; pixel_y < matrix_data.height(); pixel_y++)
        {
            if (matrix_data.get(pixel_x,pixel_y) == true)
            {
                data_matrix.at<uint8_t>(cv::Point(pixel_x,pixel_y)) = 0;
            }
        }
    }
    return data_matrix;
};

bool improc::DataMatrixDrawer::Verify(const cv::Mat& drawer_output, const std::optional<std::string>& message)
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
        return converter.to_bytes(result.text()) == message.value();
    }
    else
    {
        return false;
    }
}