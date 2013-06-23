#ifndef CDAQ_GUI_GUI_HH
#define CDAQ_GUI_GUI_HH

#include <QMainWindow>
#include <QListWidget>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include "cdaq/daq/daq.hh"
#include "cdaq/camera/camera.hh"
#include <boost/array.hpp>
#include "cdaq/daq/daq.hh"
#include "cdaq/daq/data_to_csv.hh"
#include "cdaq/misc/date.hh"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <boost/thread.hpp>
#include <QPushButton>
#include <QLineEdit>

namespace cdaq {
    
class CdaqGui
    :
    public QMainWindow
{
    Q_OBJECT
 public:
    CdaqGui(std::string &port = std::string("com3"), const int &hz = 1000, 
        const unsigned int &cam_num = 0, 
        const unsigned int &width = 640, const unsigned int &height = 480);
    ~CdaqGui();    
    
 private slots:
    void GetDaqData();
    void DrawImage(Image img);
    void ToggleSaving(bool capture);
 private:
    void SetPlotList();
    void CameraCaptureLoop();
    void StartCameraCapturing();
    void StopCameraCapturing();
 private:
    Date record_start_date_;
    Daq daq_;
    Camera camera_;
    DataToCsv daq_saver_;
    
    QwtPlot *plot_;
    boost::array<QwtPlotCurve*, Daq::kNumberOfChannels> curves_;
    QListWidget *plot_list_;
    QGraphicsPixmapItem *image_;
    QGraphicsScene *scene_;
    QGraphicsView *view_;
    QPushButton *button_capture_;
    QLineEdit *edit_folder_;
    QLineEdit *edit_csv_filename_;
    QLineEdit *edit_tag_;
    
    boost::thread thread_capture_;
    bool continue_capturing_;
    bool image_needs_updating_;
    boost::mutex mutex_needs_updating_;
    bool save_data_;
}; //class CdaqGui
    
} //namespace cdaq

#endif //ifndef CDAQ_GUI_GUI_HH
