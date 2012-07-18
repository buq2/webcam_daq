#include "cdaq/camera/camera.hh"
#include "cdaq/image/image_opencv.hh"
#include <iostream>

Camera(const int &camera_number)
    :
    source_(camera_number)
{
    if (!source_.isOpened()) {
        std::cerr << "Failed to open camera: " << camera_number << std::endl;
        abort();
    }
}
    
~Camera()
{
}
    
Image GetNextImage()
{
    Mat frame;
    source_ >> frame;
    timestamp_ = Date::Now();
    return MatToImage(frame);
}
    
Date GetPreviousTimestamp() const
{
    return timestamp_;
}
