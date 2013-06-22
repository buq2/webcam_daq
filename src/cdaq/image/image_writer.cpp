#include "cdaq/image/image_writer.hh"

using namespace cdaq;

ImageWriter::ImageWriter()
    :
    folder_("."),
    tag_("img"),
    extension_("tif"),
    continue_saving_(false),
    number_of_threads_(2),
    queue_images_(true)
{
    StartSaving();
}

ImageWriter::~ImageWriter()
{
    StopSaving();
}

void ImageWriter::AddImage(const Image &img)
{
    boost::mutex::scoped_lock lock(mutex_images_);
    images_.push(img);
}

void ImageWriter::StartSaving()
{
    if (continue_saving_) {
        // Already saving
        return;
    }
    
    continue_saving_ = true;
    
    for (unsigned int ii = 0; ii < number_of_threads_; ++ii) {
        image_saving_threads_.create_thread(boost::bind(&ImageWriter::ImageSavingLoop, this));
    }
}

void ImageWriter::StopSaving()
{
    continue_saving_ = false;
    image_saving_threads_.join_all();
}

void ImageWriter::ImageSavingLoop()
{
    while(continue_saving_) {
        Image newimg;
        {
            boost::mutex::scoped_lock lock(mutex_images_);
            
            if (images_.empty()) {
                lock.unlock();
                boost::this_thread::sleep(boost::posix_time::millisec(100));
                continue;
            }
            
            newimg = images_.front();
            images_.pop();
        }
        
        newimg.WriteWithTimestamp(folder_, tag_, extension_);
    }
}

void ImageWriter::SetTag(const std::string &tag)
{
    tag_ = tag;
}

void ImageWriter::SetFolder(const std::string &folder)
{
    folder_ = folder;
}

void ImageWriter::SetExtension(const std::string &extension)
{
    extension_ = extension;
}

void ImageWriter::SetEnableImageQueueing(const bool &queue_images)
{
    queue_images_ = queue_images;
}
