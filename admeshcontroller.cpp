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
    mode = 0;
    m_scale = 1.0;
    versor[0] = 1.0;
    versor[1] = 1.0;
    versor[2] = 1.0;
    useVersor = false;
    x_rot = 0.0;
    y_rot = 0.0;
    z_rot = 0.0;
    x_translate = 0.0;
    y_translate = 0.0;
    z_translate = 0.0;
    rel_translate = true;
    fixall_flag = false;
    exact_flag = false;
    tolerance_flag = false;
    tolerance = 0.0;
    increment_flag = false;
    increment = 0.0;
    nearby_flag = false;
    iterations = 1;
    remove_unconnected_flag = false;
    fill_holes_flag = false;
    normal_directions_flag = false;
    normal_values_flag = false;
    reverse_all_flag = false;
}

admeshController::~admeshController()
{
    delete stl;
    active = NULL;
}

void admeshController::setMode(int m)
{
    mode = m;
}

void admeshController::drawAll(QGLShaderProgram *program)
{
    if(mode == 0){
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        program->setUniformValue("color", GREEN);
        program->setUniformValue("badColor", RED);
        if(stl) stl->drawGeometry(program);
    }else if(mode == 1){
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        program->setUniformValue("color", BLACK);
        program->setUniformValue("badColor", BLACK);
        if(stl) stl->drawGeometry(program);
    }else if(mode == 2){
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        program->setUniformValue("color", BLACK);
        program->setUniformValue("badColor", BLACK);
        if(stl) stl->drawGeometry(program);
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glEnable( GL_POLYGON_OFFSET_FILL );
        glPolygonOffset( 1, 1 );
        program->setUniformValue("color", GREEN);
        program->setUniformValue("badColor", RED);
        if(stl) stl->drawGeometry(program);
    }
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

QString admeshController::getInfo()
{
    if(active) return active->getInfo();
    else return "";
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
    reCalculatePosition();
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
    reCalculatePosition();
    reDrawSignal();
}

void admeshController::saveAs()
{
    QString filter="STL_ascii (*.stl)";
    QString fileName=QFileDialog::getSaveFileName(NULL, _("Save as"), "/", _("STL_ascii (*.stl);;STL_binary (*.stl)"), &filter);
    if(!fileName.isEmpty() && active){
        fileName=fileName.section(".",0,0);
        char *file = QStringToChar(fileName+".stl");
        if(filter == "STL_ascii (*.stl)"){
            active->saveAs(file, 1);
        }else if(filter == "STL_binary (*.stl)"){
            active->saveAs(file, 2);
        }
        delete []file;
    }
}

void admeshController::exportSTL()
{
    QString filter="OBJ (*.obj)";
    QString fileName=QFileDialog::getSaveFileName(NULL, _("Export as"), "/", _("OBJ (*.obj);;OFF (*.off);;DXF (*.dxf);;VRML (*.vrml)"), &filter);
    char *file = NULL;
    if(!fileName.isEmpty() && active){
        fileName=fileName.section(".",0,0);
        if(filter == "OBJ (*.obj)"){
            file = QStringToChar(fileName+".obj");
            active->exportSTL(file, 1);
        }else if(filter == "OFF (*.off)"){
            file = QStringToChar(fileName+".off");
            active->exportSTL(file, 2);
        }else if(filter == "DXF (*.dxf)"){
            file = QStringToChar(fileName+".dxf");
            active->exportSTL(file, 3);
        }else if(filter == "VRML (*.vrml)"){
            file = QStringToChar(fileName+".vrml");
            active->exportSTL(file, 4);
        }
    delete []file;
    }
}

QVector3D admeshController::getMinPosition()
{
    if(active) return active->getMin();
    else return QVector3D(0.0,0.0,0.0);
}

void admeshController::setScale(double param)
{
    m_scale = (float)param;
}

void admeshController::setVersorX(double factor)
{
    versor[0]=factor;
}

void admeshController::setVersorY(double factor)
{
    versor[1]=factor;
}

void admeshController::setVersorZ(double factor)
{
    versor[2]=factor;
}

void admeshController::setVersor()
{
    useVersor = !useVersor;
}

void admeshController::scale()
{
    if(active && useVersor) active->scale(versor);
    else if(active) active->scale(m_scale);
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

void admeshController::setXTranslate(double factor)
{
    x_translate = factor;
}

void admeshController::setYTranslate(double factor)
{
    y_translate = factor;
}

void admeshController::setZTranslate(double factor)
{
    z_translate = factor;
}

void admeshController::setRelativeTranslate()
{
    if(rel_translate) rel_translate = false;
    else rel_translate = true;
}

void admeshController::translate()
{
    if(active) active->translate(rel_translate, x_translate, y_translate, z_translate);
}


void admeshController::setFixAllFlag()
{
    fixall_flag = !fixall_flag;
}

void admeshController::setExactFlag()
{
    exact_flag = !exact_flag;
}

void admeshController::setToleranceFlag()
{
    tolerance_flag = !tolerance_flag;
}

void admeshController::setTolerance(double val)
{
    tolerance = val;
}

void admeshController::setIncrementFlag()
{
    increment_flag = !increment_flag;
}

void admeshController::setIncrement(double val)
{
    increment = val;
}

void admeshController::setNearbyFlag()
{
    nearby_flag = !nearby_flag;
}

void admeshController::setIterations(int val)
{
    iterations = val;
}

void admeshController::setRemoveUnconnectedFlag()
{
    remove_unconnected_flag = !remove_unconnected_flag;
}

void admeshController::setFillHolesFlag()
{
    fill_holes_flag = !fill_holes_flag;
}

void admeshController::setNormalDirFlag()
{
    normal_directions_flag = !normal_directions_flag;
}

void admeshController::setNormalValFlag()
{
    normal_values_flag = !normal_values_flag;
}

void admeshController::setReverseAllFlag()
{
    reverse_all_flag = !reverse_all_flag;
}

void admeshController::repair()
{
    if(active) active->repair(fixall_flag,
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
                              reverse_all_flag);
}
