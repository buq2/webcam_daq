#ifndef CDAQ_DAQ_DATA_TO_CSV_HH
#define CDAQ_DAQ_DATA_TO_CSV_HH

#include "cdaq/daq/daq.hh"
#include <string>
#include <iostream>
#include <fstream>

namespace cdaq {
    
class CDAQDAQAPI DataToCsv
{
 public:
    DataToCsv(const std::string &outfname)
    {
        file_.open(outfname);
    }
    ~DataToCsv()
    {
        file_.close();
    }
    
    template<typename T>
    void AddData(const T &data)
    {
        file_ << data;
    }
    
    void AddData(const char *data) 
    {
        AddData(std::string(data));
    }
    
    /*
    void AddData(const char &data) 
    {

        AddData(std::string(data));
    }
    */
    
    template <typename SampleT>
    void AddData(const boost::tuple<Date, SampleT> &data)
    {
        AddData(data.get<0>().Microseconds());
        const SampleT sample = data.get<1>();
        for (unsigned int ii = 0; ii < sample.size(); ++ii) {
            AddData(",");
            AddData(sample[ii]);
        }
    }
    
    template <typename DatadSampleT>
    void AddData(const std::vector<DatadSampleT> &data) 
    {
        for (unsigned int ii = 0; ii < data.size(); ++ii) {
            AddData(data[ii]);
            AddData('\n');
        }
    }
 private:
    std::ofstream file_;
}; //class DataToCsv

} //namespace cdaq

#endif //ifndef CDAQ_DAQ_DATA_TO_CSV_HH
