#include <QApplication>
#include "gui/gui.hh"

using namespace cdaq;

int main(int argc, char *argv[])
{
    QApplication app (argc, argv);
    
    CdaqGui gui;
    gui.show();
    
    int code = app.exec();
    return code;
}
