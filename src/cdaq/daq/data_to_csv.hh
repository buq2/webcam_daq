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
        Open(outfname);
    }
    DataToCsv()
    {
    }
    
    void Open(const std::string &outfname)
    {
        if (file_.is_open()) {
            Close();
        }
        file_.open(outfname);
    }
    
    void Close()
    {
        file_.close();
    }
    
    ~DataToCsv()
    {
        Close();
    }
    
    template<typename T>
    void AddData(const T &data)
    {
        file_ << data;
        // Make sure data is written -> if program is forcibly closed
        // at least some of the data will remain
        // Slow, but, but...
        file_.flush(); 
    }
    
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
