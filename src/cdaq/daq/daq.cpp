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
	device_(io_service_),
{
	
}

bool Daq::Open()
{
	
}

void Daq::Close()
{
	
}

void Daq::StartCapturing()
{
	
}

void Daq::StopCapturing()
{
	stop_capturing_ = true;
}

void Daq::CapturingLoop()
{
	while (!stop_capturing_) {
		//Capture
	}
}

void GetBufferedData(std::vector<ElementType> *values, Date *begin, Date *end)
{
	if (NULL == values || NULL == begin || NULL == end) {
		//Can not do anything
		return;
	}
	
	boost::mutex::scoped_lock lock(mutex_data_);
	*begin = date_begin_;
	*end = data_end_;
	*values = captured_values_;
}
	
size_t NumberOfBufferedSamples() const
{
	return captured_values_.size();
}
