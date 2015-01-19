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
    stl_file* getSTLPointer();
    void drawAll(QGLShaderProgram *program);

signals:
    void reDrawSignal();

public slots:
    void openSTL();
    void openSTLbyName(const char* file);

private:
    MeshObject* stl;
    MeshObject* active;
};

#endif // ADMESHCONTROLLER_H
