#ifndef CDAQ_CAMERA_CAMERA_HH
#define CDAQ_CAMERA_CAMERA_HH

//Macros for exporting/importing functions/etc. These are needed so
//that we can call certain functions in compiled dll/static library.
//Define '*_EXPORTS' is added by CMake.
#if !defined(CDAQ_STATIC_LIBRARIES) && defined(_WIN32)
#    ifdef cdaqCamera_EXPORTS
         //Creating a library, export (make visible)
#        define CDAQCAMERAAPI __declspec(dllexport)
#    else
         //Using the library, import (find from library)
#        define CDAQCAMERAAPI __declspec(dllimport)
#    endif
#else //Not a Windows platform (probably gcc), no need for import/export
      //declarations
#    define CDAQCAMERAAPI
#endif

#include "cdaq/image/image.hh"
#include "cdaq/image/image_writer.hh"
#include "opencv2/opencv.hpp"
#include "cdaq/misc/date.hh"

namespace cdaq {
    
/// \class Camera 8-bit, RGBRGB camera. Uses OpenCV
class CDAQCAMERAAPI Camera
{
 public:
    /// Create camera from certain camera
    /// \param[in] camera_number Index of the camera
    Camera(const int &camera_number);
    
    /// Destructor
    ~Camera();
    
    /// Get newest image and send it to ImageWriter
    Image GetNextImage();
    
    /// Set capture size
    bool SetSize(const int &width, const int &height);
    
    /// Get image writer which saves the images
    ImageWriter *GetImageWriter();
 private:
    // Video source from which to capture the image
    cv::VideoCapture source_;
    
    // Image writer which is used to save the images
    ImageWriter writer_;
}; //class Camera
    
} //namespace cdaq

#endif //ifndef CDAQ_CAMERA_CAMERA_HH
