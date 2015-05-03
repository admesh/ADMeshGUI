#-------------------------------------------------
#
# Project created by QtCreator 2014-10-31T11:28:53
#
#-------------------------------------------------

QT       += core gui opengl widgets svg

lessThan(QT_MAJOR_VERSION, 5) {
   error(ADMeshGUI requires Qt 5.4 to run. Older version detected.)
}

equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 4) {
   error(ADMeshGUI requires Qt 5.4 to run. Older version detected.)
}

TARGET = admeshgui
TEMPLATE = app

load(uic)
uic.commands += -tr _

SOURCES += main.cpp\
        window.cpp \    
    renderingwidget.cpp \
    admeshcontroller.cpp \
    meshobject.cpp \
    historylist.cpp \
    propertiesdialog.cpp

HEADERS  += window.h \
    data.h \
    renderingwidget.h \
    admeshcontroller.h \
    meshobject.h \
    historylist.h \
    propertiesdialog.h \
    admeshEventFilter.h

FORMS    += window.ui \
    propertiesdialog.ui

LIBS += -ladmesh -lstlsplit
macx {
    LIBS += -lintl
    TARGET = ADMeshGUI
    ICON = Resources/admeshgui.icns
    QMAKE_INFO_PLIST = Info.plist
}

DISTFILES += \
    fshader.glsl \
    vshader.glsl

RESOURCES += \
    shaders.qrc \
    Resources.qrc

OTHER_FILES += \
    picking_vshader.glsl \
    picking_fshader.glsl \
    Info.plist

bin.files += admeshgui
bin.path = /usr/bin
mainico.files += Resources/admeshgui.svg
mainico.path = /usr/share/icons/hicolor/scalable/apps
16ico.files += Resources/16x16/admeshgui.png
16ico.path = /usr/share/icons/hicolor/16x16/apps
32ico.files += Resources/32x32/admeshgui.png
32ico.path = /usr/share/icons/hicolor/32x32/apps
48ico.files += Resources/48x48/admeshgui.png
48ico.path = /usr/share/icons/hicolor/48x48/apps
desktop.files += admeshgui.desktop
desktop.path = /usr/share/applications
INSTALLS += bin desktop mainico 16ico 32ico 48ico


