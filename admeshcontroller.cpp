#include "admeshcontroller.h"
#include <QFileDialog>
#include <QtWidgets>
#include <string>

using namespace std;

admeshController::admeshController(QObject *parent) :
    QObject(parent)
{
    stl = new stl_file;
    active = new stl_file;
    stl_initialize(stl);
    active = NULL;
}

admeshController::~admeshController()
{
    stl_close(stl);
    active = NULL;
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
        char *file = QStringToChar(fileName);
        stl_open(stl, file);
        if(stl_get_error(stl)){
           QString msg;
           QTextStream(&msg) << _("File ") << fileName << _(" could not be opened.\n");
           QMessageBox::critical(NULL, _("Error"), msg);
         stl_clear_error(stl);
        }
        delete []file;
        active = stl;
    }
    reDrawSignal();
}

void admeshController::openSTLbyName(const char* filename){
    char* file = const_cast<char *>(filename);
    stl_open(stl, file);
    if(stl_get_error(stl)){
       QString msg;
       QTextStream(&msg) << _("File ") << file << _(" could not be opened.\n");
       QMessageBox::critical(NULL, _("Error"), msg);
     stl_clear_error(stl);
    }
    active = stl;
    reDrawSignal();
}

stl_file* admeshController::getSTLPointer()
{
    return stl;
}
