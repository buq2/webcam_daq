#include <boost/lexical_cast.hpp>
#include "cdaq/camera/camera.hh"
#include <iostream>
#include <string>
#include <boost/thread.hpp>

using namespace cdaq;

int main(int argc, char *argv[])
{
    if (argc < 6) {
        std::cerr << "Requires 5 inputs <camnum>,<width>,<height>,<num captured>,<folder>" << std::endl;
        return -1;
    }
    
    const std::string camnum_str = argv[1];
    const std::string width_str = argv[2];
    const std::string height_str = argv[3];
    const std::string num_captured_str = argv[4];
    const std::string folder = argv[5];
    
    int camnum = 0;
    int width = 0;
    int height = 0;
    int num_captured = 0;
    try {
        camnum = boost::lexical_cast<int>(camnum_str);
        width = boost::lexical_cast<int>(width_str);
        height = boost::lexical_cast<int>(height_str);
        num_captured = boost::lexical_cast<int>(num_captured_str);
    } catch(...) {
        std::cerr << "Failed to convert camnum/width/height/num_captured to int" << std::endl;
        return -1;
    }

    
    Camera cam(camnum);
    bool success = cam.SetSize(width,height);
    if (!success) {
        std::cerr << "Failed to set size" << std::endl;
        return -1;
    }
    
    cam.GetImageWriter()->SetFolder(folder);

    for (unsigned int ii = 0; ii < num_captured; ++ii) {
        Image img = cam.GetNextImage();
    }
    
    return 0;
}
