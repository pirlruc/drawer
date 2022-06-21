#include <improc/drawer/drawer_types/data_matrix_drawer.hpp>

improc::DataMatrixDrawer::DataMatrixDrawer(): improc::BaseDrawer() 
                                            , writer_(ZXing::DataMatrix::Writer()) {}

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
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter {};
    ZXing::BitMatrix matrix_data = this->writer_.encode ( converter.from_bytes(message.value())
                                                        , improc::DataMatrixDrawer::kMinWidth
                                                        , improc::DataMatrixDrawer::kMinHeight );
    cv::Mat data_matrix = 255 * cv::Mat::ones(matrix_data.height(),matrix_data.width(),improc::DataMatrixDrawer::kImageDataType);
    // TODO: Improve method to draw barcode using threads
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
