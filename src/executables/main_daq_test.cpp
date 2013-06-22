#include <boost/lexical_cast.hpp>
#include "cdaq/daq/daq.hh"
#include <iostream>
#include <string>
#include <boost/thread.hpp>

using namespace cdaq;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cerr << "Required two inputs <port>, <hz>" << std::endl;
        return -1;
    }
    
    const std::string port = argv[1];
    const std::string hz_str = argv[2];
    
    int hz = 0;
    try {
        hz = boost::lexical_cast<int>(hz_str);
    } catch(...) {
        std::cerr << "Failed to convert hz to int" << std::endl;
        return -1;
    }
    
    Daq daq(port, hz);
    daq.Open();
    daq.StartCapturing();
    
    boost::this_thread::sleep(boost::posix_time::millisec(static_cast<boost::int64_t>(1000*1)));
    daq.StopCapturing();
    
    return 0;
}
