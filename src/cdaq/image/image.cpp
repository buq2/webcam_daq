#include "cdaq/image/image.hh"
#include <cstring> //memcpy

using namespace cdaq;

Image::Image(const int &width, const int &height)
    :
    buffer_(boost::shared_array<unsigned char>(new unsigned char[width*height*3])),
    width_(width),
    height_(height)
{
    
}

Image::Image(const unsigned char *data, const int &width, const int &height)
    :
    buffer_(boost::shared_array<unsigned char>(new unsigned char[width*height*3])),
    width_(width),
    height_(height)
{
    memcpy(buffer_.get(), data, this->BufferSize());  
}

Image::~Image()
{
    
}
