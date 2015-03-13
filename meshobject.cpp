#include "meshobject.h"
#include <string>
#include <QVector3D>

using namespace std;

MeshObject::MeshObject()
{
    stl = new stl_file;
    stl_initialize(stl);
    active = true;
    saved = true;
    file = NULL;
    references = 0;
}

MeshObject::MeshObject(const MeshObject& m)
{
    //vbo = m.vbo;
    references = 0;
    file = (char*)malloc(strlen(m.file)+1);
    strcpy(file, m.file);
    file = m.file;
    saved = false;
    active = m.active;
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
    /*for(int i=0; i<stl->stats.number_of_facets * 3;i++){
        stl->edge_start[i] = m.stl->edge_start[i];
    }*/
    initializeGLFunctions();
    glGenBuffers(1, &vbo);
    this->updateGeometry();
    /*stl_verify_neighbors(stl);
    stl_generate_shared_vertices(stl);*/
}

MeshObject::~MeshObject(){
    stl_close(stl);
    delete(stl);
    glDeleteBuffers(1, &vbo);
    //free(file);
}

bool MeshObject::loadGeometry(char* fileName)
{
    stl_open(stl, fileName);
    if(stl_get_error(stl)){
       stl_clear_error(stl);
       return false;
    }
    stl_repair(stl,0,1,0,0,0,0,0,0,0,0,0,0,0,0);
    stl_calculate_volume(stl);
    initializeGLFunctions();
    glGenBuffers(1, &vbo);
    this->updateGeometry();
    file = fileName;
    return true;
}

bool MeshObject::hasValidName()
{
    if(!file)return false;
    else if(strlen(file)<5)return false;
    else return true;
}

bool MeshObject::isSaved()
{
    return saved;
}

void MeshObject::saveAs(char* filename, int type)
{
    if(type == 1){
        stl_write_ascii(stl, filename, "ADMeshSTLmodel");
    }else if(type == 2){
        stl_write_binary(stl, filename, "ADMeshSTLmodel");
    }
    delete []file;
    file = filename;
    saved = true;
}

void MeshObject::save()
{
    if(stl->stats.type == binary){
        stl_write_binary(stl, file, "ADMeshSTLmodel");
    }else if(stl->stats.type == ascii){
        stl_write_ascii(stl, file, "ADMeshSTLmodel");
    }
    saved = true;
}

void MeshObject::exportSTL(char* filename, int type)
{
    stl_check_facets_exact(stl);
    stl_generate_shared_vertices(stl);
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

void MeshObject::scale(float factor)
{
    stl_scale(stl, factor);
    this->updateGeometry();
}

void MeshObject::scale(float versor[3])
{
    stl_scale_versor(stl, versor);
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
}

void MeshObject::setActive()
{
    active = true;
}

void MeshObject::setInactive()
{
    active = false;
}

void MeshObject::toggleActive()
{
    active = !active;
}

bool MeshObject::isActive()
{
    return active;
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
    saved = false;
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


