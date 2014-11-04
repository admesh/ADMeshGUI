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
    src/connect.c \
    src/normals.c \
    src/shared.c \
    src/stl_io.c \
    src/stlinit.c \
    src/util.c \
    renderingwidget.cpp

HEADERS  += window.h \
    data.h \
    src/stl.h \
    renderingwidget.h

FORMS    += window.ui


