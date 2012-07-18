#ifndef CDAQ_IMAGE_IMAGE_HH
#define CDAQ_IMAGE_IMAGE_HH

//Macros for exporting/importing functions/etc. These are needed so
//that we can call certain functions in compiled dll/static library.
//Define '*_EXPORTS' is added by CMake.
#if defined (_WIN32) 
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

#include <boost/shared_array.hpp>

namespace cdaq {
    
/// \class Image Class for storing 8bit color images. Supports only
///     packet (RGBRGB) images.
class CDAQIMAGEAPI Image
{
 public:
    /// Create empty image
    /// \param[in] width Width of the image in pixels
    /// \param[in] height Height of the image in pixels
    Image(const int &width, const int &height);
    
    /// Create image from buffer. Data will be copied
    /// \param[in] buffer Buffer from which the data should be copied
    /// \param[in] width Width of the image in pixels
    /// \param[in] height Height of the image in pixels
    Image(const unsigned char *buffer, const int &width, const int &height);
    
    /// Destructor
    ~Image();
    
    /// \return Width of the image in pixels
    int width() const {return width_;}
    
    /// \return Height of the image in pixels
    int height() const {return height_;}
    
    /// \return Buffer size in bytes
    // Number of pixels is multiplied with 3 as there is three channels.
    unsigned long BufferSize() const {return width()*height()*3;}
    
    /// \return Pointer to the start of the image buffer
    unsigned char pointer() const {return buffer.get()};

 private:
    //Buffer which contains the image data
    boost::shared_array<unsigned char> buffer_;
    
    //Width of the image
    int width_;
    
    //Height of the image
    int height_;
}; //class Image

} //namespace cdaq

#endif //ifndef CDAQ_IMAGE_IMAGE_HH
