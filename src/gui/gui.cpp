#include "gui/gui.hh"
#include <qwt_plot_grid.h>
#include <QSplitter>
#include <QWidget>
#include <QHBoxLayout>
#include <Qtimer>
#include <QColor>
#include <QPixmap>
#include <QMetaType>
#include <boost/bind.hpp>
#include "opencv2/opencv.hpp"
#include <QLabel>
#include <QSpacerItem>
#include <boost/filesystem.hpp>

using namespace cdaq;

CdaqGui::CdaqGui(std::string &port, const int &hz, const unsigned int &cam_num, const unsigned int &width, const unsigned int &height)
    :
    daq_(port, hz),
    camera_(cam_num),
    continue_capturing_(false),
    image_needs_updating_(true)
{
    camera_.SetSize(width, height);
    qRegisterMetaType<cdaq::Image>("Image");
    
    // Basic qwt items
    // First the plot canvase
    plot_ = new QwtPlot();
    plot_->setCanvasBackground(Qt::white);
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->attach(plot_);
    plot_->setAutoReplot(false);
    plot_->setAxisAutoScale(QwtPlot::xBottom, false); //Calculated tightly from the data
    plot_->setAxisAutoScale(QwtPlot::yLeft, true);
    
    // Curves for the plot
    boost::array<Qt::GlobalColor, 8> colors = {{Qt::blue, Qt::red, Qt::green, Qt::cyan, Qt::magenta, Qt::black, Qt::yellow , Qt::gray}};
    for (unsigned int ii = 0; ii < Daq::kNumberOfChannels; ++ii) {
        curves_[ii] = new QwtPlotCurve();

        const int color_num = ii%colors.size();
        curves_[ii]->setPen(QColor(colors[color_num]),1.5);
        curves_[ii]->attach(plot_);
        curves_[ii]->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    }
    
    // Now actual Qt widgets to which the plots will connect
    QSplitter *splitter_horizontal = new QSplitter(this);
    QSplitter *splitter_vertical = new QSplitter(this);
    
    QWidget *left = new QWidget(splitter_horizontal);
    QWidget *top = new QWidget(splitter_vertical);
    QWidget *bottom = new QWidget(splitter_vertical);
    QWidget *central = new QWidget(this);
    QHBoxLayout *central_layout = new QHBoxLayout(central);
    QHBoxLayout *layout_top = new QHBoxLayout(top);
    QHBoxLayout *layout_bottom = new QHBoxLayout(bottom);
    QVBoxLayout *layout_left = new QVBoxLayout(left);
    plot_list_ = new QListWidget(bottom);
    button_capture_ = new QPushButton(left);
    button_capture_->setText("Toggle saving");
    button_capture_->setCheckable(true);
    
    // Different labels/edit
    QWidget *widget_folder = new QWidget(left);
    QWidget *widget_csv = new QWidget(left);
    QWidget *widget_tag = new QWidget(left);
    widget_folder->setLayout(new QHBoxLayout());
    widget_csv->setLayout(new QHBoxLayout());
    widget_tag->setLayout(new QHBoxLayout());
    
    QLabel *label_folder = new QLabel(widget_folder);
    QLabel *label_tag = new QLabel(widget_csv);
    QLabel *label_csv = new QLabel(widget_tag);
    edit_folder_ = new QLineEdit(left);
    edit_csv_filename_ = new QLineEdit(left);
    edit_tag_ = new QLineEdit(left);
    label_folder->setText("Output folder:");
    label_csv->setText("CSV filename:");
    label_tag->setText("Img prefix:");
    edit_folder_->setText("results");
    edit_tag_->setText("cam");
    edit_csv_filename_->setText("data.csv");
    
    // Image viewing related
    view_ = new QGraphicsView(top);
    scene_ = new QGraphicsScene(view_);
    view_->setScene(scene_);
    // Default image is just an empty image
    image_ = scene_->addPixmap(QPixmap()); 
    

    // Create layout
    this->setCentralWidget(central);
    central->setLayout(central_layout);
    central_layout->addWidget(splitter_horizontal);
    splitter_horizontal->addWidget(left);
    splitter_horizontal->addWidget(splitter_vertical);
    splitter_vertical->addWidget(top);
    splitter_vertical->addWidget(bottom);
    top->setLayout(layout_top);
    bottom->setLayout(layout_bottom);
    left->setLayout(layout_left);
    layout_left->addWidget(button_capture_);
    
    layout_left->addWidget(widget_folder);
    layout_left->addWidget(widget_csv);
    layout_left->addWidget(widget_tag);
    layout_left->addItem(new QSpacerItem(0,0,QSizePolicy::Minimum, QSizePolicy::Expanding));
    
    widget_folder->layout()->addWidget(label_folder);
    widget_folder->layout()->addWidget(edit_folder_);
    widget_csv->layout()->addWidget(label_csv);
    widget_csv->layout()->addWidget(edit_csv_filename_);
    widget_tag->layout()->addWidget(label_tag);
    widget_tag->layout()->addWidget(edit_tag_);
    
    
    
    splitter_vertical->setOrientation(Qt::Vertical);
    
    // Add actual GUI components
    layout_bottom->addWidget(plot_list_);
    layout_bottom->addWidget(plot_);
    layout_top->addWidget(view_);
    
    // Daq related
    daq_.Open();
    daq_.StartCapturing();
    
    // Init
    SetPlotList();
    
    // Buttons
    connect(button_capture_, SIGNAL(clicked(bool)), this, SLOT(ToggleSaving(bool)));
    
    ToggleSaving(false);
    StartCameraCapturing();

    // Timers for acquiring daq data
    QTimer *timer_daq = new QTimer(this);
    connect(timer_daq, SIGNAL(timeout()), this, SLOT(GetDaqData()));
    timer_daq->start(500);
}

void CdaqGui::ToggleSaving(bool capture)
{
    if (capture) {
        std::string tag = edit_tag_->text().toUtf8().constData();
        std::string folder = edit_folder_->text().toUtf8().constData();
        std::string csv = edit_csv_filename_->text().toUtf8().constData();
        
        boost::filesystem::create_directories(boost::filesystem::path(folder));
        daq_saver_.Open(folder + "/" + csv);
        camera_.GetImageWriter()->SetFolder(folder);
        camera_.GetImageWriter()->SetTag(tag);
    }
    edit_tag_->setEnabled(!capture);
    edit_folder_->setEnabled(!capture);
    edit_csv_filename_->setEnabled(!capture);

    save_data_ = capture;
    camera_.GetImageWriter()->SetEnableImageQueueing(capture);
}

void CdaqGui::SetPlotList()
{
    plot_list_->setSelectionMode(QAbstractItemView::MultiSelection);
    plot_list_->setMaximumWidth(100);
    for (unsigned int ii = 0; ii < Daq::kNumberOfChannels; ++ii) {
        std::string plotname = "Channel " + boost::lexical_cast<std::string>(ii);
        plot_list_->addItem(plotname.c_str());
        plot_list_->item(ii)->setSelected(true);
    }
}

CdaqGui::~CdaqGui()
{
    StopCameraCapturing();
}

void CdaqGui::GetDaqData()
{
    std::vector<Daq::DatedSampleType> samples;
    daq_.GetBufferedDataAndClear(&samples);
    
    if (save_data_) {
        daq_saver_.AddData(samples);
    }
    
    boost::int64_t micros_begin = record_start_date_.Microseconds();
    QVector<double> xdata;
    boost::array<QVector<double>, Daq::kNumberOfChannels> ydata;

    double x_min = 0;
    double x_max = 0;

    for (size_t ii = 0; ii < samples.size(); ++ii) {
        const Date timestamp = samples[ii].get<0>();
        double times = (double)(timestamp.Microseconds() - micros_begin)/(1000.0*1000.0);
        if (ii == 0) {
            x_min = times;
        } else if (ii == samples.size()-1) {
            x_max = times;
        }

        xdata.push_back(times);

        for (unsigned int chan = 0; chan < Daq::kNumberOfChannels; ++chan) {
            ydata[chan].push_back(samples[ii].get<1>()[chan]);
        }
    }
    
    for (unsigned int chan = 0; chan < Daq::kNumberOfChannels; ++chan) {
        curves_[chan]->setSamples(xdata, ydata[chan]);

        if (plot_list_->item(chan)->isSelected()) {
            curves_[chan]->show();
        } else {
            curves_[chan]->hide();
        }
    }

    plot_->setAxisScale(QwtPlot::xBottom, x_min, x_max);
    plot_->replot();
}


void CdaqGui::CameraCaptureLoop()
{
    while(continue_capturing_) {
        Image img = camera_.GetNextImage();
        boost::mutex::scoped_lock lock(mutex_needs_updating_);
        if (image_needs_updating_) {
            QMetaObject::invokeMethod(this, "DrawImage", Qt::AutoConnection, Q_ARG(Image, img));
            image_needs_updating_ = false;
        }
    }
}

void CdaqGui::StartCameraCapturing()
{
    continue_capturing_ = true;
    thread_capture_ = boost::thread(boost::bind(&CdaqGui::CameraCaptureLoop, this));
}

void CdaqGui::StopCameraCapturing()
{
    continue_capturing_ = false;
    thread_capture_.join();
}

void CdaqGui::DrawImage(Image img)
{
    cv::Mat mat = img.ToOpencvImage();
    const int width = img.Width();
    const int height = img.Height();
    const int bytes_per_line = mat.step;
    
    // Need to use QImage as bytes_per_line is probably aligned to certain number
    // of bytes (non continuous data)
    QImage qimg(mat.ptr(), width, height, bytes_per_line, QImage::Format_RGB888);
    image_->setPixmap(QPixmap::fromImage(qimg.rgbSwapped()));
    view_->fitInView(scene_->itemsBoundingRect(),Qt::KeepAspectRatio);
    
    boost::mutex::scoped_lock lock(mutex_needs_updating_);
    image_needs_updating_ = true;
}
