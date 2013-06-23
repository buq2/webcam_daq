#ifndef CDAQ_DAQ_DAQ_HH
#define CDAQ_DAQ_DAQ_HH

//Macros for exporting/importing functions/etc. These are needed so
//that we can call certain functions in compiled dll/static library.
//Define '*_EXPORTS' is added by CMake.
#if !defined(CDAQ_STATIC_LIBRARIES) && defined(_WIN32) 
#    ifdef cdaqDaq_EXPORTS
         //Creating a library, export (make visible)
#        define CDAQDAQAPI __declspec(dllexport)
#    else
         //Using the library, import (find from library)
#        define CDAQDAQAPI __declspec(dllimport)
#    endif
#else //Not a Windows platform (probably gcc), no need for import/export
      //declarations
#    define CDAQDAQAPI
#endif

#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/shared_array.hpp>
#include <boost/array.hpp>
#include <boost/tuple/tuple.hpp>

#include "cdaq/misc/daTe.hh"


namespace cdaq {
    
class CDAQDAQAPI Daq
{
 public:
    /// Create daq object and set connection parameters
    /// \param[in] com_port Serial port which to connect
    /// \param[in] hz Sampling rate which will be set to the daq
    Daq(const std::string &com_port, const int &hz);
    ~Daq();

    /// Connect to daq and start capturing.
    /// \return True if conncetion to the daq was successfull
    // Also starts the capturing thread
    bool Open();
    
    /// \return True if open
    bool IsOpen()
    {
        return device_open_;
    }
    
    /// Stop capturing
    void Close();
 
    /// How many channels this daq has
    static const int kNumberOfChannels = 8;
    
    /// Type of the captured values
    typedef boost::uint16_t ElementType;
    
    /// Type of single sample
    typedef boost::array<ElementType, kNumberOfChannels> SampleType;
    
    /// Type of single sample with date code
    typedef boost::tuple<Date, SampleType> DatedSampleType;
    
    /// Get captured data. Note that buffers are not cleared
    /// \param[out] values Captured values with date
    void GetBufferedData(std::vector<DatedSampleType> *values);
    
    /// Get captured data and clear buffers such that next time the function
    /// is called no overlapping data is returned
    /// \param[out] values Captured values with date
    void GetBufferedDataAndClear(std::vector<DatedSampleType> *values);
    
    /// \return Number of captured  samples
    size_t NumberOfBufferedSamples();
    
    /// Start capturing. Creates capturing thread
    void StartCapturing();
    
    /// Stops capturing. Capturing thread will be destroyed.
    void StopCapturing();
    
    /// \return True if Daq configured successfully. Must be called after
    ///     configuration message has been sent.
    bool CheckConfiguration();
 private:
    /// Flush serial port data
    void Flush();
 
    /// Data points are captured  in this function. Function should be
    /// called only by the capturing thread.
    void CapturingLoop();
    
    /// Get raw data from serial device
    /// \param[out] bytes Number bytes in returned array
    /// \return Raw data from the serial device
    boost::shared_array<boost::uint8_t> GetRawSerialData(boost::uint32_t *bytes);
    
    /// Fill raw_buffer with new data
    void FillRawBuffer();

    size_t BytesWaiting();
    
    void ProcessRawBuffer();
    
    /// Transform raw bytes from the daq to correct byte order
    ElementType DaqRawToNative(const ElementType &elem);
    
    /// Add data to raw buffer
    void AddDataToBuffer(boost::uint8_t *ptr, size_t num_bytes);
 private:

    
    // Serial device string ("COM1", "/dev/ttyUSB0")
    std::string device_string_;
    
    // Sampling rate in HZ
    int hz_;
    
    // True if device has been opened
    bool device_open_;

    // IO-service which was used to create the serial object
    boost::asio::io_service io_service_;
    
    // Serial device object
    boost::asio::serial_port device_;
    
    // Thread which captures the data from the serial object
    boost::thread acquisition_thread_;
    
    // If true, capturing will be stopped
    bool stop_capturing_;
    
    // How long the capturing thread sleeps after it has processed all 
    // data from the serial object
    double capturing_thread_sleep_period_;
    
    // Date of the first element of the 'raw_buffer_'
    Date date_begin_;
    
    // Approx date of the last element of the 'raw_buffer_'
    Date date_end_;
    
    // Mutex which protects the captured_values_
    boost::mutex mutex_data_;
    
    // Raw data buffer to which all serial data is captured
    std::vector<boost::uint8_t> raw_buffer_;
    
    // Ready samples
    std::vector<DatedSampleType> data_buffer_;
    
}; //class Daq
    
} //namespace cdaq

#endif //ifndef CDAQ_DAQ_DAQ_HH
