#ifndef CDAQ_GUI_GUI_HH
#define CDAQ_GUI_GUI_HH

#include <QMainWindow>
#include <QListWidget>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include "cdaq/daq/daq.hh"
#include <boost/array.hpp>
#include "cdaq/daq/daq.hh"
#include "cdaq/misc/date.hh"

namespace cdaq {
    
class CdaqGui
    :
    public QMainWindow
{
    Q_OBJECT
 public:
    CdaqGui();
    ~CdaqGui();
    
    QwtPlot *plot_;
    boost::array<QwtPlotCurve*, Daq::kNumberOfChannels> curves_;
    QListWidget *plot_list_;
    
    Daq daq_;
 private slots:
    void GetDaqData();
 private:
    void SetPlotList();
 private:
    Date record_start_date_;
}; //class CdaqGui
    
} //namespace cdaq

#endif //ifndef CDAQ_GUI_GUI_HH
