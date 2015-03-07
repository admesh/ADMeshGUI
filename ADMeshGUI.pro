#-------------------------------------------------
#
# Project created by QtCreator 2014-10-31T11:28:53
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = ADMeshGUI
TEMPLATE = app

load(uic)
uic.commands += -tr _

SOURCES += main.cpp\
        window.cpp \    
    renderingwidget.cpp \
    admeshcontroller.cpp \
    meshobject.cpp

HEADERS  += window.h \
    data.h \
    renderingwidget.h \
    admeshcontroller.h \
    meshobject.h

FORMS    += window.ui

LIBS += -ladmesh -L -lglut -lGLU

DISTFILES += \
    fshader.glsl \
    vshader.glsl

RESOURCES += \
    shaders.qrc

OTHER_FILES += \
    picking_vshader.glsl \
    picking_fshader.glsl




