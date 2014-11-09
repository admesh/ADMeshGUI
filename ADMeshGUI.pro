#-------------------------------------------------
#
# Project created by QtCreator 2014-10-31T11:28:53
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ADMeshGUI
TEMPLATE = app

SOURCES += main.cpp\
        window.cpp \    
    renderingwidget.cpp \
    admeshcontroller.cpp

HEADERS  += window.h \
    data.h \
    renderingwidget.h \
    admeshcontroller.h

FORMS    += window.ui

LIBS += -ladmesh -L -lglut -lGLU




