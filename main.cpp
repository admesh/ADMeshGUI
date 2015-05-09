// (c) 2015 David Vyvlečka, AGPLv3
/*[Information about Qt + OpenGL classes (e.g. how to use the API) obtained from Qt Reference Pages
 http://doc.qt.io/qt-5/reference-overview.html 9. 5. 2015]*/

#include <QApplication>
#include <QDesktopWidget>

#include "window.h"
#include "data.h"
#include "admeshEventFilter.h"

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

#ifdef Q_OS_MAC
    app.installEventFilter(new admeshEventFilter(&app, &window));
#endif

    window.setWindowIcon(QIcon::fromTheme("admeshgui", QIcon("://Resources/admeshgui.svg")));
    window.resize(window.sizeHint());
    int desktopArea = QApplication::desktop()->width() *
                     QApplication::desktop()->height();
    int widgetArea = window.width() * window.height();

    window.setWindowTitle("ADMeshGUI");

    if (((float)widgetArea / (float)desktopArea) < 0.75f)
        window.show();
    else
        window.showMaximized();

    for(int i=1;i<argc;i++){
        window.openByFilename(argv[i]);
    }
    return app.exec();
}
