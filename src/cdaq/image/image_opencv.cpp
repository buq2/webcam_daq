#include "cdaq/image/image_opencv.hh"
#include <iostream>

using namespace cv;

namespace cdaq {

/// Convert CDAQ Image to OpenCV matrix
/// \param[in] img Image to be copied to OpenCV matrix
/// \return OpenCV matrix
cv::Mat ImageToMat(const Image &img)
{
    const int height = img.Height();
    const int width = img.Width();
    
    // Always 8bit RGBRGB
    Mat cv_img(Size(width, height), CV_8UC3);
    
    unsigned char *ptr = img.Pointer();
    
    for (int y = 0; y < height; ++y) {
        memcpy(cv_img.ptr(y), ptr, width*3);
        ptr += width*3;
    }
    
    return cv_img;
}

/// Convert OpenCV matrix to CDAQ Image
/// \return CDAQ Image
Image MatToImage(const cv::Mat &cv_img)
{
    if (cv_img.type() != CV_8UC3) {
        std::cerr << "OpenCV matrix has wrong type" << std::endl;
        abort();
    }
    
    const int height = cv_img.rows;
    const int width = cv_img.cols;
    
    Image img(width, height);
    unsigned char *ptr = img.Pointer();
    
    for (int y = 0; y < height; ++y) {
        memcpy(ptr, cv_img.ptr(y), width*3);
        ptr += width*3;
    }
    
    return img;
}

} //namespace cdaq
