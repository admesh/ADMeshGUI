#include "meshobject.h"
#include <string>
#include <QVector3D>

using namespace std;

struct VertexData
{
    QVector3D position;
    QVector3D normal;
    VertexData() {}
    VertexData( const QVector3D pos, const QVector3D norm) : position(pos), normal(norm) {}
};

MeshObject::MeshObject()
{
    stl = new stl_file;
    stl_initialize(stl);
}

MeshObject::~MeshObject(){
    stl_close(stl);
    delete(stl);
    glDeleteBuffers(1, &vbo);
}

bool MeshObject::loadGeometry(char* fileName)
{
    stl_open(stl, fileName);
    if(stl_get_error(stl)){
       stl_clear_error(stl);
       return false;
    }

    initializeGLFunctions();
    glGenBuffers(1, &vbo);
    this->updateGeometry();
    return true;
}

void MeshObject::scale(float factor)
{
    stl_scale(stl, factor);
    this->updateGeometry();
}

void MeshObject::mirrorXY()
{
    stl_mirror_xy(stl);
    this->updateGeometry();
}

void MeshObject::mirrorYZ()
{
    stl_mirror_yz(stl);
    this->updateGeometry();
}

void MeshObject::mirrorXZ()
{
    stl_mirror_xz(stl);
    this->updateGeometry();
}

void MeshObject::rotateX(float angle)
{
    stl_rotate_x(stl, angle);
    this->updateGeometry();
}

void MeshObject::rotateY(float angle)
{
    stl_rotate_y(stl, angle);
    this->updateGeometry();
}

void MeshObject::rotateZ(float angle)
{
    stl_rotate_z(stl, angle);
    this->updateGeometry();
}

void MeshObject::translate(bool relative, float x_trans, float y_trans, float z_trans)
{
    if(relative){
        stl_translate_relative(stl, x_trans, y_trans, z_trans);
    }else{
        stl_translate(stl, x_trans, y_trans, z_trans);
    }
    this->updateGeometry();
}

void MeshObject::updateGeometry()
{
    int N = stl->stats.number_of_facets;

    GLfloat *vertices;
    vertices=new GLfloat[N*18];
    for(int i=0;i<N;i++){
        int index = i*18;
        vertices[index]=stl->facet_start[i].vertex[0].x;
        vertices[index+1]=stl->facet_start[i].vertex[0].y;
        vertices[index+2]=stl->facet_start[i].vertex[0].z;
        vertices[index+3]=stl->facet_start[i].normal.x;
        vertices[index+4]=stl->facet_start[i].normal.y;
        vertices[index+5]=stl->facet_start[i].normal.z;

        vertices[index+6]=stl->facet_start[i].vertex[1].x;
        vertices[index+7]=stl->facet_start[i].vertex[1].y;
        vertices[index+8]=stl->facet_start[i].vertex[1].z;
        vertices[index+9]=stl->facet_start[i].normal.x;
        vertices[index+10]=stl->facet_start[i].normal.y;
        vertices[index+11]=stl->facet_start[i].normal.z;

        vertices[index+12]=stl->facet_start[i].vertex[2].x;
        vertices[index+13]=stl->facet_start[i].vertex[2].y;
        vertices[index+14]=stl->facet_start[i].vertex[2].z;
        vertices[index+15]=stl->facet_start[i].normal.x;
        vertices[index+16]=stl->facet_start[i].normal.y;
        vertices[index+17]=stl->facet_start[i].normal.z;
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, N * 18 * sizeof(GLfloat), vertices, GL_DYNAMIC_DRAW);
    delete [] vertices;
}

void MeshObject::drawGeometry(QGLShaderProgram *program)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, 0);

    int normalLocation = program->attributeLocation("a_normal");
    program->enableAttributeArray(normalLocation);
    glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, (const void *)(sizeof(GLfloat)*3));

    glDrawArrays(GL_TRIANGLES, 0, stl->stats.number_of_facets*3);
}


