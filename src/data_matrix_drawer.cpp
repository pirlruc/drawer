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
    cv::Mat data_matrix (matrix_data.height(),matrix_data.width(),improc::BaseDrawer::kImageDataType);
    // TODO: Move image conversion method to a base image class
    auto bitmatrix_begin = matrix_data.row(0).begin();
    auto bitmatrix_end   = matrix_data.row(matrix_data.height()).end();
    std::transform  ( bitmatrix_begin,bitmatrix_end,data_matrix.begin<uint8_t>()
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