#ifndef CDAQ_IMAGE_IMAGE_WRITER_HH
#define CDAQ_IMAGE_IMAGE_WRITER_HH

#include "cdaq/image/image.hh"
#include <queue>
#include <boost/thread.hpp>

namespace cdaq {

class CDAQIMAGEAPI ImageWriter
{
 public:
    ImageWriter();
    ~ImageWriter();
    
    /// Add image to be saved
    void AddImage(const Image &img);
    
    /// Set tag for images 
    void SetTag(const std::string &tag);
    
    /// Set folder to which the images are saved
    void SetFolder(const std::string &folder);
    
    /// Set extension which is used when images are saved. Determines
    /// filetype
    void SetExtension(const std::string &extension);
    
    /// Set if AddImage function adds images to be written, or simply discards them
    void SetEnableImageQueueing(const bool &queue_images);
 private:
    /// Start saving images. Creates threads
    void StartSaving();
    
    /// Stops saving images. Joins threads
    void StopSaving();
    
    /// Function in which threads save the images
    void ImageSavingLoop();
 private:
    // Queued images
    std::queue<Image> images_;
    
    // Path to which the images are saved
    std::string folder_;
    
    // Tag which to use
    std::string tag_;
    
    // File extension
    std::string extension_;
    
    // Mutex protecting images
    boost::mutex mutex_images_;
    
    // Threads used to save images
    boost::thread_group image_saving_threads_;
    
    // If false, image saving threads will stop
    bool continue_saving_;
    
    // How many threads to use for saving
    unsigned int number_of_threads_;
    
    // If false, images are not queued/written
    bool queue_images_;
}; //class ImageWriter

} //namespace cdaq

#endif //ifndef CDAQ_IMAGE_IMAGE_WRITER_HH
