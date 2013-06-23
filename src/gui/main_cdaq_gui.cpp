#include <QApplication>
#include "gui/gui.hh"
#include <iostream>
#include <boost/lexical_cast.hpp>

using namespace cdaq;

void Help()
{
    std::cout << "executable [com_port] [sampling_rate_hz] [camera_number] [image_width] [image_height]" << std::endl;
}

unsigned int ParseArg(char *arg)
{
    unsigned int out = 0;
    try {
        out = boost::lexical_cast<unsigned int>(std::string(arg));
    } catch(...) {
        Help();
        exit(-1);
    }
    
    return out;
}

int main(int argc, char *argv[])
{
    std::string port("com3");
    unsigned int hz = 1000;
    unsigned int cam_num = 0;
    unsigned int width = 640;
    unsigned int height = 480;
    
    if (argc == 1) {
        std::cout << "You should really consider using the args:" << std::endl;
        Help();
    } 
    if (argc>1) {
        port = argv[1];
    }
    if (argc>2) {
        hz = ParseArg(argv[2]);
    }
    if (argc>3) {
        cam_num = ParseArg(argv[3]);
    }
    if (argc>4) {
        width = ParseArg(argv[4]);
    }
    if (argc>5) {
        height = ParseArg(argv[5]);
    }
    
    std::cout << "Starting program with options:" << std::endl;
    std::cout << "Com port: " << port << std::endl;
    std::cout << "Sampling rate: " << hz << std::endl;
    std::cout << "Camera number: " << cam_num << std::endl;
    std::cout << "Image size: " << width << "x" << height << std::endl;
    
    QApplication app (argc, argv);
    
    CdaqGui gui(port, hz, cam_num, width, height);
    gui.show();
    
    int code = app.exec();
    return code;
}
