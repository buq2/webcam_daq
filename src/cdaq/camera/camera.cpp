#include "cdaq/camera/camera.hh"
#include "cdaq/image/image_opencv.hh"
#include <iostream>

using namespace cdaq;
using namespace cv;

Camera::Camera(const int &camera_number)
    :
    source_(camera_number)
{
    if (!source_.isOpened()) {
        std::cerr << "Failed to open camera: " << camera_number << std::endl;
        abort();
    }
}
    
Camera::~Camera()
{
}
    
Image Camera::GetNextImage()
{
    
    
    Mat frame;
    source_ >> frame;
    timestamp_ = Date::Now();
    return MatToImage(frame);
}
    
Date Camera::GetPreviousTimestamp() const
{
    return timestamp_;
}

bool Camera::SetSize(const int &width, const int &height)
{
    bool success1 = source_.set(CV_CAP_PROP_FRAME_WIDTH, width);
    bool success2 = source_.set(CV_CAP_PROP_FRAME_HEIGHT, height);
    
    return success1 && success2;
}
