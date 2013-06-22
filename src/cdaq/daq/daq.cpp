#include "cdaq/daq/daq.hh"
#include <boost/bind.hpp>
#include <boost/algorithm/searching/boyer_moore_horspool.hpp>

using namespace cdaq;
using boost::asio::detail::socket_ops::network_to_host_long;
using boost::asio::detail::socket_ops::host_to_network_long;
using boost::asio::detail::socket_ops::network_to_host_short;
using boost::asio::detail::socket_ops::host_to_network_short;

//Tries to find byte pattern form of {0xFF,0xFF,0xFF,marker};
static boost::uint8_t *FindByteFlag(boost::uint8_t *str,
                                      const boost::uint32_t &lenstr,
                                      boost::uint8_t marker)
{
    boost::uint8_t pat[4];
    pat[0] = 0xFF;
    pat[1] = 0xFF;
    pat[2] = 0xFF;
    pat[3] = marker;
    boost::uint8_t *str_end = str+lenstr;
    boost::uint8_t *found_ptr = boost::algorithm::boyer_moore_horspool_search(str, str_end, pat, pat+4);
    if (found_ptr == str_end) {
        // Not found
        return NULL;
    }
    return found_ptr;
}

//At the time of the writing boost endian library was not in the official boost libraries
static boost::uint16_t native_to_little(const boost::uint16_t &a)
{
    //To big endian
    boost::uint16_t b = host_to_network_short(a); //Network order is big-endian
    //Swap bytes
    boost::uint8_t *ptr = (boost::uint8_t*)&b;
    boost::uint8_t tmp = ptr[0];
    ptr[0] = ptr[1];
    ptr[1] = tmp;
    return b;
}

static boost::uint16_t little_to_native(const boost::uint16_t &a)
{
    boost::uint16_t b = a;
    //To big endian
    //Swap bytes
    boost::uint8_t *ptr = (boost::uint8_t*)&b;
    boost::uint8_t tmp = ptr[0];
    ptr[0] = ptr[1];
    ptr[1] = tmp;

    //Now from big endian to native
    return network_to_host_short(b);
}

Daq::Daq(const std::string &com_port, const int &hz)
    :
    device_string_(com_port),
    hz_(hz),
    device_(io_service_),
    device_open_(false),
    capturing_thread_sleep_period_(2.0)
{
    
}

Daq::~Daq()
{
}

bool Daq::Open()
{
    if (device_open_) {
        // Already open
        return true;
    }
    
    device_.open(device_string_);
    device_.set_option(boost::asio::serial_port_base::baud_rate(230400));
    device_.set_option(boost::asio::serial_port_base::character_size(8));
    device_.set_option(boost::asio::serial_port_base::flow_control( boost::asio::serial_port_base::flow_control::none));
    device_.set_option(boost::asio::serial_port_base::parity( boost::asio::serial_port_base::parity::none));
    device_.set_option(boost::asio::serial_port_base::stop_bits( boost::asio::serial_port_base::stop_bits::one));
    
    device_open_ = true;
    
    //Set properties: Start configuration (4+1 byte), set sampling rate (2 byte)
    const int confdata_size = 4+1+2;
    boost::uint8_t confdata[confdata_size] = {0xFF, 0xFF, 0xFF, 0x20, 0x01, 0x00, 0x00};
    unsigned short int hz16 = native_to_little(hz_);
    memcpy(&confdata[5], &hz16, 2);

    //Remove unwanted data from serial buffer (there should be none)
    Flush();   

    //Send configuration message
    boost::asio::write(device_,boost::asio::buffer(confdata,confdata_size));
    
    const bool configuration_successfull = CheckConfiguration();

    if (!configuration_successfull) {
        std::cerr << "Daq8: Configuration failed" << std::endl;
        return false;
    }
    
    return true;
}

bool Daq::CheckConfiguration()
{
    //Now we wait for a little while to get some data to serial buffer
    boost::this_thread::sleep(boost::posix_time::millisec(100));

    boost::uint32_t bytes = 0;
    boost::shared_array<boost::uint8_t> readbuf = GetRawSerialData(&bytes);

    //OK and failure patterns
    const boost::uint8_t okConf = 0x30;
    const boost::uint8_t failedConf = 0x31;

    //Check if failure
    boost::uint8_t *failed_ptr = FindByteFlag(&readbuf[0], bytes, failedConf);
    if (failed_ptr != NULL) {
        std::cerr << "Daq: Device configuration failed (received failed pattern)" << std::endl;
        return false;
    }

    //Check if ok
    boost::uint8_t *ok_ptr = FindByteFlag(&readbuf[0], bytes, okConf);
    if (ok_ptr == NULL) {
        std::cout << "Daq: Could not find 'configuration-ok'-message after configuring the device" << std::endl;
        return false; //Not OK
    }

    std::cout << "Daq: Configuration successfull" << std::endl;
    
    // Add rest of the data to buffer
    //AddDataToBuffer(ok_ptr, bytes-(&readbuf[0]-ok_ptr));
    Flush();

    //Success
    return true;
}

void Daq::Close()
{
}

void Daq::StartCapturing()
{
    StopCapturing();
    
    //Create capturing thread
    stop_capturing_ = false;
    acquisition_thread_ = boost::thread(boost::bind(&Daq::CapturingLoop, this));
}

void Daq::StopCapturing()
{
    stop_capturing_ = true;
    acquisition_thread_.join();
}

void Daq::CapturingLoop()
{
    //Clear captured values and raw buffer, this is the "time" for the
    //first captured value.
    data_buffer_.clear();
    raw_buffer_.clear();
    Flush();
    
    while (!stop_capturing_) {        
        date_begin_ = date_end_;
        while(raw_buffer_.size() < (4+sizeof(ElementType)*kNumberOfChannels)*capturing_thread_sleep_period_*kNumberOfChannels) {
            FillRawBuffer();
        }
        ProcessRawBuffer();
    }
}

void Daq::GetBufferedData(std::vector<DatedSampleType> *values)
{
    if (NULL == values) {
        //Can not do anything
        return;
    }
    
    boost::mutex::scoped_lock lock(mutex_data_);
    *values = data_buffer_;
}
    
size_t Daq::NumberOfBufferedSamples() const
{
    return data_buffer_.size();
}

size_t Daq::BytesWaiting()
{
    boost::asio::serial_port::native_handle_type handle = device_.native_handle();

    size_t nbytes = 0;
    #if defined(BOOST_WINDOWS)
        // Uhh, ohh. Don't know how to read available bytes in Windows.
        // I thought that you could use the following:
        //   DWORD junk;
        //   DeviceIoControl(handle, 
        //           FIONREAD, 
        //           NULL,0,
        //           &nbytes, sizeof(nbytes),
        //           &junk,
        //           NULL
        //           );
        // But apparently this does not work. We can also lie and say that
        // there is always ~100kb of data.
        nbytes = 1024*100;
    #else
        ioctl(handle, FIONREAD, (char*)&nbytes);
    #endif
    
    return nbytes;
}

boost::shared_array<boost::uint8_t> Daq::GetRawSerialData(boost::uint32_t *bytes)
{
    *bytes = BytesWaiting();
    boost::shared_array<boost::uint8_t> readbuf(new boost::uint8_t[*bytes]);
    
    if (0 != *bytes) {
        date_end_ = Date::Now();
        *bytes = device_.read_some(boost::asio::buffer(&(readbuf[0]),*bytes));
    }
    return readbuf;
}

void Daq::FillRawBuffer()
{
    //Fetch new data from the serial port
    boost::uint32_t bytes = 0;
    
    boost::shared_array<boost::uint8_t> buf = GetRawSerialData(&bytes);
    AddDataToBuffer(&buf[0], bytes);
}

void Daq::AddDataToBuffer(boost::uint8_t *ptr, size_t num_bytes)
{
    for (boost::uint32_t ii = 0; ii < num_bytes; ++ii) {
        raw_buffer_.push_back(ptr[ii]);
    }
}

void Daq::ProcessRawBuffer()
{
    if (0 == raw_buffer_.size()) {
        return;
    }

    // Find where the pattern begins
    boost::uint8_t *pattern_begin = FindByteFlag(&(raw_buffer_[0]), raw_buffer_.size(), 0x10);
    const size_t num_unwanted = pattern_begin-&(raw_buffer_[0]);
    // Remove unwanted crap which can not be saved
    raw_buffer_.erase(raw_buffer_.begin(), raw_buffer_.begin()+num_unwanted);

    // How many bytes we need to completely transfer all the channels
    // 4 bytes for pattern and rest for the actual data
    const unsigned int bytes_per_sample = 4 + sizeof(ElementType)*kNumberOfChannels;

    const size_t total_bytes = raw_buffer_.size();
    boost::uint8_t *last_nonprocessed_byte = &(raw_buffer_[0]);
    std::vector<SampleType> samples;
    for(size_t ii = 0;ii + bytes_per_sample < total_bytes; ii += bytes_per_sample) {
        const boost::uint8_t *first_data_byte = &(raw_buffer_[ii+4]);
        // For each channel
        SampleType sample;
        for (unsigned int chan = 0; chan < kNumberOfChannels; ++chan) {
            ElementType elem = 0;
            memcpy(&elem, first_data_byte, sizeof(elem));
            elem = DaqRawToNative(elem);
            sample[chan] = elem;
        }
        samples.push_back(sample);
        last_nonprocessed_byte += bytes_per_sample;
    }
    
    // Now erase all data which was processed
    const size_t num_processed = last_nonprocessed_byte-&(raw_buffer_[0]);
    raw_buffer_.erase(raw_buffer_.begin(), raw_buffer_.begin()+num_processed);
    
    
    // Finally get date codes for all samples
    // Begin date is clear
    // For (hopefully) more accurate end date we need to take into
    // account that the date_end_ is for date for the next sample
    const double micros_between_samples = (date_end_.Microseconds() - date_begin_.Microseconds())/(double)samples.size();
    std::cout << micros_between_samples << std::endl;
    for (size_t ii = 0; ii < samples.size(); ++ii) {
        Date sample_date = date_begin_;
        sample_date.AddMicroseconds(micros_between_samples * ii);
        
        DatedSampleType s;
        s.get<0>() = sample_date;
        s.get<1>() = samples[ii];
        data_buffer_.push_back(s);
    }
}

void Daq::Flush()
{
    boost::uint32_t bytes = 0;
    boost::shared_array<boost::uint8_t> buf = GetRawSerialData(&bytes);
}

Daq::ElementType Daq::DaqRawToNative(const Daq::ElementType &elem)
{
    // For current implementation
    return little_to_native(elem);
}


