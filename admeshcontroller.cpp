#include "admeshcontroller.h"
#include <QFileDialog>
#include <QtWidgets>
#include <string>
//#include <QVector3D>

using namespace std;

admeshController::admeshController(QObject *parent) :
    QObject(parent)
{
    stl = NULL;
    active = NULL;
    m_scale = 1.0;
}

admeshController::~admeshController()
{
    delete stl;
    active = NULL;
}

void admeshController::drawAll(QGLShaderProgram *program)
{
     program->setUniformValue("color", color);
     program->setUniformValue("badColor", badColor);
     if(stl) stl->drawGeometry(program);
}

void admeshController::setDrawColor(QVector3D col, QVector3D badCol){
    color = col;
    badColor = badCol;
}

char* QStringToChar(QString str)
{
    string s = str.toStdString();
    char *cstr = new char[s.length() + 1];
    strcpy(cstr, s.c_str());
    return cstr;
}

void admeshController::openSTL()
{
    QString fileName = QFileDialog::getOpenFileName(NULL, _("Open STL"), "/", _("STL (*.stl)"));
    if(!fileName.isEmpty()){
        MeshObject* tmp = new MeshObject;
        char *file = QStringToChar(fileName);
        if(!tmp->loadGeometry(file)){
            QString msg;
            QTextStream(&msg) << _("File ") << fileName << _(" could not be opened.\n");
            QMessageBox::critical(NULL, _("Error"), msg);
        }else{
            stl = tmp;
            active = stl;
        }
        delete []file;
    }
    reDrawSignal();
}

void admeshController::openSTLbyName(const char* filename)
{
    char* file = const_cast<char *>(filename);
    MeshObject* tmp = new MeshObject;
    if(!tmp->loadGeometry(file)){
        QString msg;
        QTextStream(&msg) << _("File ") << file << _(" could not be opened.\n");
        QMessageBox::critical(NULL, _("Error"), msg);
    }else{
        stl = tmp;
        active = stl;
    }
    reDrawSignal();
}

void admeshController::setScale(double param)
{
    m_scale = (float)param;
}

void admeshController::scale()
{
    if(active) active->scale(m_scale);
}

void admeshController::mirrorXY()
{
    if(active) active->mirrorXY();
}

void admeshController::mirrorYZ()
{
    if(active) active->mirrorYZ();
}

void admeshController::mirrorXZ()
{
    if(active) active->mirrorXZ();
}

void admeshController::setXRot(double angle)
{
    x_rot = (float)angle;
}

void admeshController::setYRot(double angle)
{
    y_rot = (float)angle;
}

void admeshController::setZRot(double angle)
{
    z_rot = (float)angle;
}

void admeshController::rotateX()
{
    if(active) active->rotateX(x_rot);
}

void admeshController::rotateY()
{
    if(active) active->rotateY(y_rot);
}

void admeshController::rotateZ()
{
    if(active) active->rotateZ(z_rot);
}

