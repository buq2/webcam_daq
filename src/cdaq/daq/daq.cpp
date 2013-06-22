#include "cdaq/daq/daq.hh"

using namespace cdaq;
using boost::asio::detail::socket_ops::network_to_host_long;
using boost::asio::detail::socket_ops::host_to_network_long;
using boost::asio::detail::socket_ops::network_to_host_short;
using boost::asio::detail::socket_ops::host_to_network_short;

Daq::Daq(const std::string &com_port, const int &hz)
	:
	device_string_(com_port),
	hz_(hz),
	device_(io_service_)
{
	
}

bool Daq::Open()
{
	return true;
}

void Daq::Close()
{
}

void Daq::StartCapturing()
{
	StopCapturing();
	
	
	
	//Clear captured values and raw buffer, this is the "time" for the
	//first captured value.
	captured_values_.clear();
	raw_buffer_.clear();
	date_begin_ = Date::Now();
	
	//Create capturing thread
}

void Daq::StopCapturing()
{
	stop_capturing_ = true;
	acquisition_thread_.join();
}

void Daq::CapturingLoop()
{
	while (!stop_capturing_) {
		FillRawBuffer();
		date_end_ = Date::Now();
	}
}

void Daq::GetBufferedData(std::vector<ElementType> *values, Date *begin, Date *end)
{
	if (NULL == values || NULL == begin || NULL == end) {
		//Can not do anything
		return;
	}
	
	boost::mutex::scoped_lock lock(mutex_data_);
	*begin = date_begin_;
	*end = date_end_;
	*values = captured_values_;
}
	
size_t Daq::NumberOfBufferedSamples() const
{
	return captured_values_.size();
}

size_t BytesWaiting()
{
    return 0;
}

boost::shared_array<boost::uint8_t> Daq::GetRawSerialData(boost::uint32_t *bytes)
{
    *bytes = BytesWaiting();
    boost::shared_array<boost::uint8_t> readbuf(new boost::uint8_t[*bytes]);
    *bytes = device_.read_some(boost::asio::buffer(&readbuf[0],*bytes));
    return readbuf;
}

void Daq::FillRawBuffer()
{
	//Fetch new data from the serial port
	boost::uint32_t bytes = 0;
	
	boost::shared_array<boost::uint8_t> buf = GetRawSerialData(&bytes);
	for (boost::uint32_t ii = 0; ii < bytes; ++ii) {
		raw_buffer_.push_back(buf[ii]);
	}
}






