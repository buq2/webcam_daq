#include "gui/gui.hh"
#include <qwt_plot_grid.h>
#include <QSplitter>
#include <QWidget>
#include <QHBoxLayout>
#include <Qtimer>
#include <QColor>

using namespace cdaq;

CdaqGui::CdaqGui()
    :
    daq_("com3", 1000)
{
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
    QSplitter *splitter = new QSplitter(this);
    QWidget *top = new QWidget(splitter);
    QWidget *bottom = new QWidget(splitter);
    QWidget *central = new QWidget(this);
    plot_list_ = new QListWidget(bottom);
    QHBoxLayout *central_layout = new QHBoxLayout(central);
    QHBoxLayout *layout_top = new QHBoxLayout(top);
    QHBoxLayout *layout_bottom = new QHBoxLayout(bottom);

    // Create layout
    this->setCentralWidget(central);
    central->setLayout(central_layout);
    central_layout->addWidget(splitter);
    splitter->addWidget(top);
    splitter->addWidget(bottom);
    top->setLayout(layout_top);
    bottom->setLayout(layout_bottom);
    
    splitter->setOrientation(Qt::Vertical);
    
    // Add actual GUI components
    layout_bottom->addWidget(plot_list_);
    layout_bottom->addWidget(plot_);
    
    // Daq related
    daq_.Open();
    daq_.StartCapturing();
    
    // Init
    SetPlotList();
    
    // Timers for acquiring data
    QTimer *timer_daq = new QTimer(this);
    connect(timer_daq, SIGNAL(timeout()), this, SLOT(GetDaqData()));
    timer_daq->start(500);
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
}

void CdaqGui::GetDaqData()
{
    std::vector<Daq::DatedSampleType> samples;
    daq_.GetBufferedDataAndClear(&samples);
    
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

