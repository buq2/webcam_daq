#include "cdaq/camera/camera.hh"
#include <iostream>
#include <boost/lexical_cast.hpp>

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
    writer_.SetTag(std::string("cam") + boost::lexical_cast<std::string>(camera_number));
}
    
Camera::~Camera()
{
}
    
Image Camera::GetNextImage()
{
    // Get image from OpenCV
    Mat frame;
    source_ >> frame;
    
    // Use this moment as timestamp for the image
    const Date timestamp = Date::Now();
    
    // Convert to internal format and add correct timestamp
    Image img(frame);
    img.SetDate(timestamp);
    
    // Add image to writer
    writer_.AddImage(img);
    
    return img;
}
    
bool Camera::SetSize(const int &width, const int &height)
{
    bool success1 = source_.set(CV_CAP_PROP_FRAME_WIDTH, width);
    bool success2 = source_.set(CV_CAP_PROP_FRAME_HEIGHT, height);
    
    return success1 && success2;
}

ImageWriter *Camera::GetImageWriter()
{
    return &writer_;
}
