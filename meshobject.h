#ifndef MESHOBJECT_H
#define MESHOBJECT_H

#include <QGLFunctions>
#include <QGLShaderProgram>
#include "data.h"

class MeshObject : protected QGLFunctions
{
public:
    MeshObject();
    bool loadGeometry(char* filename);
    void updateGeometry();
    void drawGeometry(QGLShaderProgram *program);
    virtual ~MeshObject();

private:
    stl_file* stl;
    GLuint vbo;
};

#endif // MESHOBJECT_H
