#include "cdaq/camera/camera.hh"
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
    const Date timestamp = Date::Now();
    Image img(frame);
    img.SetDate(timestamp);
    return img;
}
    
bool Camera::SetSize(const int &width, const int &height)
{
    bool success1 = source_.set(CV_CAP_PROP_FRAME_WIDTH, width);
    bool success2 = source_.set(CV_CAP_PROP_FRAME_HEIGHT, height);
    
    return success1 && success2;
}
