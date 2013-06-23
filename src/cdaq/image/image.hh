#ifndef CDAQ_IMAGE_IMAGE_HH
#define CDAQ_IMAGE_IMAGE_HH

//Macros for exporting/importing functions/etc. These are needed so
//that we can call certain functions in compiled dll/static library.
//Define '*_EXPORTS' is added by CMake.
#if !defined(CDAQ_STATIC_LIBRARIES) && defined(_WIN32)
#    ifdef cdaqImage_EXPORTS
         //Creating a library, export (make visible)
#        define CDAQIMAGEAPI __declspec(dllexport)
#    else
         //Using the library, import (find from library)
#        define CDAQIMAGEAPI __declspec(dllimport)
#    endif
#else //Not a Windows platform (probably gcc), no need for import/export
      //declarations
#    define CDAQIMAGEAPI
#endif

#include "opencv2/opencv.hpp"
#include "cdaq/misc/date.hh"

namespace cdaq {
    
/// \class Image Class for storing 8bit color images. Supports only
///     packet (RGBRGB) images.
class CDAQIMAGEAPI Image
{
 public:
    Image()
    {
    }
    
    /// Create empty image
    /// \param[in] width Width of the image in pixels
    /// \param[in] height Height of the image in pixels
    Image(const int &width, const int &height);
    
    /// Create image from OpenCV data
    Image(cv::Mat mat);
    
    /// Destructor
    ~Image();
    
    /// \return Width of the image in pixels
    int Width() const {return width_;}
    
    /// \return Height of the image in pixels
    int Height() const {return height_;}
    
    void WriteWithTimestamp(const std::string &folder, const std::string &tag, const std::string &extension);
    
    void SetDate(const Date &date)
    {
        date_ = date;
    }
    
    Date GetDate() const
    {
        return date_;
    }
    
    cv::Mat ToOpencvImage()
    {
        return buffer_;
    }
 private:
    //Buffer which contains the image data
    cv::Mat buffer_;
    
    //Width of the image
    int width_;
    
    //Height of the image
    int height_;
    
    // Date of the image
    Date date_;
}; //class Image

} //namespace cdaq

#endif //ifndef CDAQ_IMAGE_IMAGE_HH
