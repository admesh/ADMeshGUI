#include "meshobject.h"
#include <string>
#include <QVector3D>

using namespace std;

char* QStringToChar(QString str)
{
    string s = str.toStdString();
    char *cstr = new char[s.length() + 1];
    strcpy(cstr, s.c_str());
    return cstr;
}

MeshObject::MeshObject()
{
    stl = new stl_file;
    stl_initialize(stl);
    selected = true;
    saved = true;    
    resetFilename();
    references = 0;
    size = 0;
    hidden = false;
}

MeshObject::MeshObject(stl_file* item)
{
    stl = item;
    selected = true;
    saved = false;
    references = 0;
    size = 0;
    hidden = false;
    file = QString("split");
    stl_calculate_volume(stl);
    initializeGLFunctions();
    glGenBuffers(1, &vbo);
    this->updateGeometry();
}

MeshObject::MeshObject(const MeshObject& m) : QGLFunctions()
{
    references = 0;
    hidden = m.hidden;
    file = m.file;
    saved = m.saved;
    selected = m.selected;
    stl = new stl_file;
    stl_initialize(stl);
    stl->fp = m.stl->fp;
    stl->stats = m.stl->stats;
    stl->M = m.stl->M;
    stl->error = m.stl->error;
    stl_reallocate(stl);
    for(int i=0; i<stl->stats.number_of_facets;i++){
        stl->facet_start[i] = m.stl->facet_start[i];
        stl->neighbors_start[i] = m.stl->neighbors_start[i];
    }
    initializeGLFunctions();
    glGenBuffers(1, &vbo);
    this->updateGeometry();
}

MeshObject::~MeshObject(){
    stl_close(stl);
    delete(stl);
    glDeleteBuffers(1, &vbo);
}

bool MeshObject::loadGeometry(QString fileName)
{
    char* filename = QStringToChar(fileName);
    stl_open(stl, filename);
    if(stl_get_error(stl)){
       stl_clear_error(stl);
       return false;
    }
    delete []filename;
    stl_repair(stl,0,1,0,0,0,0,0,0,0,0,0,0,0,0);
    stl_calculate_volume(stl);
    initializeGLFunctions();
    glGenBuffers(1, &vbo);
    this->updateGeometry();
    file = fileName;
    return true;
}

void MeshObject::resetFilename()
{
    file = "untitled";
}

bool MeshObject::hasValidName()
{
    if(file.size() < 5)return false;
    else if(file == "untitled")return false;
    else if(file == "split")return false;
    else return true;
}

stl_file* MeshObject::getStlPointer()
{
    return stl;
}

bool MeshObject::isSaved()
{
    return saved;
}

void MeshObject::saveAs(QString fileName, int type)
{
    char* filename = QStringToChar(fileName);
    if(type == 1){
        stl_write_ascii(stl, filename, "ADMeshSTLmodel");
    }else if(type == 2){
        stl_write_binary(stl, filename, "ADMeshSTLmodel");
    }
    delete []filename;
    file = fileName;
    saved = true;
}

void MeshObject::save()
{
    char* filename = QStringToChar(file);
    if(stl->stats.type == binary){
        stl_write_binary(stl, filename, "ADMeshSTLmodel");
    }else if(stl->stats.type == ascii){
        stl_write_ascii(stl, filename, "ADMeshSTLmodel");
    }
    delete []filename;
    saved = true;
}

void MeshObject::exportSTL(QString fileName, int type)
{
    stl_check_facets_exact(stl);
    stl_generate_shared_vertices(stl);
    char* filename = QStringToChar(fileName);
    char label[] = "ADMeshDXFexport";
    if(type == 1){
        stl_write_obj(stl, filename);
    }else if(type == 2){
        stl_write_off(stl, filename);
    }else if(type == 3){
        stl_write_dxf(stl, filename, label);
    }else if(type == 4){
        stl_write_vrml(stl, filename);
    }
    delete []filename;
}

QVector3D MeshObject::getMin()
{
    QVector3D min = QVector3D(
                    stl->stats.min.x,
                    stl->stats.min.y,
                    stl->stats.max.z
                );
    return min;
}

float MeshObject::getDiameter()
{
    float *arr = new float[6];
    float val = 0.0;
    arr[0] = qAbs(stl->stats.min.x);
    arr[1] = qAbs(stl->stats.min.y);
    arr[2] = qAbs(stl->stats.min.z);
    arr[3] = qAbs(stl->stats.max.x);
    arr[4] = qAbs(stl->stats.max.y);
    arr[5] = qAbs(stl->stats.max.z);
    for(int i=0;i<6;i++){
        if (arr[i] > val) val = arr[i];
    }
    return val;
}

float* MeshObject::getInfo()
{
    float *arr = new float[15];
    arr[0] = stl->stats.min.x;
    arr[1] = stl->stats.min.y;
    arr[2] = stl->stats.min.z;
    arr[3] = stl->stats.max.x;
    arr[4] = stl->stats.max.y;
    arr[5] = stl->stats.max.z;
    arr[6] = (float)stl->stats.number_of_facets;
    arr[7] = (float)stl->stats.degenerate_facets;
    arr[8] = (float)stl->stats.edges_fixed;
    arr[9] = (float)stl->stats.facets_removed;
    arr[10] = (float)stl->stats.facets_added;
    arr[11] = (float)stl->stats.facets_reversed;
    arr[12] = (float)stl->stats.backwards_edges;
    arr[13] = (float)stl->stats.normals_fixed;
    arr[14] = stl->stats.volume;
    return arr;
}

QString MeshObject::getName()
{
    return file;
}

void MeshObject::scale(float versor[3])
{
    stl_scale_versor(stl, versor);
    this->updateGeometry();
    saved = false;
}

void MeshObject::mirrorXY()
{
    stl_mirror_xy(stl);
    this->updateGeometry();
    saved = false;
}

void MeshObject::mirrorYZ()
{
    stl_mirror_yz(stl);
    this->updateGeometry();
    saved = false;
}

void MeshObject::mirrorXZ()
{
    stl_mirror_xz(stl);
    this->updateGeometry();
    saved = false;
}

void MeshObject::rotateX(float angle)
{
    stl_rotate_x(stl, angle);
    this->updateGeometry();
    saved = false;
}

void MeshObject::rotateY(float angle)
{
    stl_rotate_y(stl, angle);
    this->updateGeometry();
    saved = false;
}

void MeshObject::rotateZ(float angle)
{
    stl_rotate_z(stl, angle);
    this->updateGeometry();
    saved = false;
}

void MeshObject::translate(bool relative, float x_trans, float y_trans, float z_trans)
{
    if(relative){
        stl_translate_relative(stl, x_trans, y_trans, z_trans);
    }else{
        stl_translate(stl, x_trans, y_trans, z_trans);
    }
    this->updateGeometry();
    saved = false;
}

void MeshObject::repair(int fixall_flag, int exact_flag, int tolerance_flag, float tolerance, int increment_flag, float increment, int nearby_flag, int iterations, int remove_unconnected_flag, int fill_holes_flag, int normal_directions_flag, int normal_values_flag, int reverse_all_flag)
{
    stl_repair(stl,
               fixall_flag,
               exact_flag,
               tolerance_flag,
               tolerance,
               increment_flag,
               increment,
               nearby_flag,
               iterations,
               remove_unconnected_flag,
               fill_holes_flag,
               normal_directions_flag,
               normal_values_flag,
               reverse_all_flag,
               0);
    stl_calculate_volume(stl);
    this->updateGeometry();
    saved = false;
}

void MeshObject::setSelected()
{
    selected = true;
}

void MeshObject::setDeselected()
{
    selected = false;
}

void MeshObject::toggleSelected()
{
    selected = !selected;
}

bool MeshObject::isSelected()
{
    return selected;
}

bool MeshObject::isActive()
{
    return(selected && !hidden);
}

void MeshObject::setHidden()
{
    hidden = true;
}

void MeshObject::setVisible()
{
    hidden = false;
}

bool MeshObject::isHidden()
{
    return hidden;
}

bool MeshObject::hasReferences()
{
    if(references > 0) return true;
    else return false;
}

void MeshObject::addReference()
{
    references++;
}

void MeshObject::removeReference()
{
    references--;
}

void MeshObject::countSize()
{
    size = 2*stl->stats.number_of_facets*SIZEOF_STL_FACET; //for facets and neighbours
    size += sizeof(stl->stats);
    size += sizeof(stl_file);
    size += sizeof(*this);
}

unsigned long MeshObject::getSize()
{
    return size;
}

void MeshObject::updateGeometry()
{
    int N = stl->stats.number_of_facets;

    countSize();
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

