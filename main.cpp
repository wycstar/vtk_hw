#include "mainwindow.h"

#include "QVTKOpenGLNativeWidget.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <QVTKOpenGLWindow.h>
#include "vtkAutoInit.h" 
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);


int main(int argc, char* argv[])
{
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLWindow::defaultFormat());
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
