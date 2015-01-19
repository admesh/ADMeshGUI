#include <QApplication>
#include <QDesktopWidget>

#include "window.h"
#include "data.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    setlocale(LC_NUMERIC,"C");

    QSurfaceFormat format;
    /*format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setSamples(4);*/
    /*format.setProfile(QSurfaceFormat::CoreProfile);*/
    //QSurfaceFormat::setDefaultFormat(format);

    Window window;
    window.resize(window.sizeHint());
    int desktopArea = QApplication::desktop()->width() *
                     QApplication::desktop()->height();
    int widgetArea = window.width() * window.height();

    window.setWindowTitle("ADMesh GUI");

    if (((float)widgetArea / (float)desktopArea) < 0.75f)
        window.show();
    else
        window.showMaximized();

    if(argv[1]){ // read input file from line
        window.openByFilename(argv[1]);
    }
    return app.exec();
}
