#ifndef ADMESHCONTROLLER_H
#define ADMESHCONTROLLER_H

#include <QGLShaderProgram>
#include <QObject>
#include "data.h"
#include "meshobject.h"

class admeshController : public QObject
{
    Q_OBJECT
public:
    explicit admeshController(QObject *parent = 0);
    ~admeshController();
    void drawAll(QGLShaderProgram *program);
    void setDrawColor(QVector3D);

signals:
    void reDrawSignal();

public slots:
    void openSTL();
    void openSTLbyName(const char* file);

private:
    MeshObject* stl;
    MeshObject* active;
    QVector3D color;
};

#endif // ADMESHCONTROLLER_H
