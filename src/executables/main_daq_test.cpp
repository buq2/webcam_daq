#include <boost/lexical_cast.hpp>
#include "cdaq/daq/daq.hh"
#include "cdaq/daq/data_to_csv.hh"
#include <iostream>
#include <string>
#include <boost/thread.hpp>

using namespace cdaq;

int main(int argc, char *argv[])
{
    if (argc < 3) {
        std::cerr << "Required three inputs <port>, <hz>, <outfname>" << std::endl;
        return -1;
    }
    
    const std::string port = argv[1];
    const std::string hz_str = argv[2];
    const std::string fname = argv[3];
    
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
    
    DataToCsv out(fname);
    
    while(true) {
        boost::this_thread::sleep(boost::posix_time::millisec(static_cast<boost::int64_t>(1000*1)));
        std::vector<Daq::DatedSampleType> samples;
        daq.GetBufferedDataAndClear(&samples);
        out.AddData(samples);
    }
    
    //daq.StopCapturing();
    
    return 0;
}
