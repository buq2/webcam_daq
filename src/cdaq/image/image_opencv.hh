#ifndef CDAQ_IMAGE_IMAGE_OPENCV_HH
#define CDAQ_IMAGE_IMAGE_OPENCV_HH

#include "cdaq/image/image.hh"
#include "opencv2/opencv.hpp"

/// \file image_opencv.hh Free functions for converting OpenCV images
///     to cdaq images and vice versa.

namespace cdaq {
    
/// Convert CDAQ Image to OpenCV matrix
/// \param[in] img Image to be copied to OpenCV matrix
/// \return OpenCV matrix
cv::Mat CDAQIMAGEAPI ImageToMat(const Image &img);

/// Convert OpenCV matrix to CDAQ Image
/// \return CDAQ Image
Image CDAQIMAGEAPI MatToImage(const cv::Mat &cv_img);
    
} //namespace cdaq

#endif //ifndef CDAQ_IMAGE_IMAGE_OPENCV_HH
