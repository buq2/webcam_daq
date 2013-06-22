#include <boost/lexical_cast.hpp>
#include "cdaq/camera/camera.hh"
#include <iostream>
#include <string>
#include <boost/thread.hpp>

using namespace cdaq;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cerr << "Required three inputs <camnum>,<width>,<height>" << std::endl;
        return -1;
    }
    
    const std::string camnum_str = argv[1];
    const std::string width_str = argv[2];
    const std::string height_str = argv[3];
    
    int camnum = 0;
    int width = 0;
    int height = 0;
    try {
        camnum = boost::lexical_cast<int>(camnum_str);
        width = boost::lexical_cast<int>(width_str);
        height = boost::lexical_cast<int>(height_str);
    } catch(...) {
        std::cerr << "Failed to convert camnum/width/height to int" << std::endl;
        return -1;
    }

    
    Camera cam(camnum);
    bool success = cam.SetSize(width,height);
    if (!success) {
        std::cerr << "Failed to set size" << std::endl;
        return -1;
    }
    Image img = cam.GetNextImage();
    std::cout << img.Width() << std::endl;
    std::cout << img.Height() << std::endl;

    boost::this_thread::sleep(boost::posix_time::millisec(static_cast<boost::int64_t>(1000*3)));
    
    return 0;
}
