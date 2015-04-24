// (c) 2015 David Vyvlečka, AGPLv3

#include <QApplication>
#include <QDesktopWidget>

#include "window.h"
#include "data.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    setlocale(LC_NUMERIC,"C");
    textdomain("admeshgui");

    QCoreApplication::setOrganizationName("ADMesh");
    QCoreApplication::setApplicationName("ADMeshGUI");

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setSamples(4);
    QSurfaceFormat::setDefaultFormat(format);

    Window window;
    window.setWindowIcon(QIcon::fromTheme("admeshgui", QIcon("://Resources/admeshgui.svg")));
    window.resize(window.sizeHint());
    int desktopArea = QApplication::desktop()->width() *
                     QApplication::desktop()->height();
    int widgetArea = window.width() * window.height();

    window.setWindowTitle("ADMesh GUI");

    if (((float)widgetArea / (float)desktopArea) < 0.75f)
        window.show();
    else
        window.showMaximized();

    for(int i=1;i<argc;i++){
        window.openByFilename(argv[i]);
    }
    return app.exec();
}
