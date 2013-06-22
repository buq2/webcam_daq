#include "cdaq/image/image.hh"
#include <boost/filesystem.hpp>

using namespace cdaq;

Image::Image(const int &width, const int &height)
    :
    buffer_(width, height, CV_8UC3),
    width_(width),
    height_(height)
{
    
}

Image::Image(cv::Mat mat)
    :
    buffer_(mat),
    height_(mat.rows),
    width_(mat.cols)
{
}

Image::~Image()
{
    
}

void Image::WriteWithTimestamp(const std::string &folder, const std::string &tag, const std::string &extension)
{
    boost::filesystem::create_directories(boost::filesystem::path(folder));
    std::string fname = folder + "/" + tag + "_" + boost::lexical_cast<std::string>(date_.Microseconds()) + "." + std::string(extension);
    imwrite(fname, buffer_);
}
