#include "admeshcontroller.h"
#include <QFileDialog>
#include <QtWidgets>
#include <string>

using namespace std;

admeshController::admeshController(QObject *parent) :
    QObject(parent)
{
    count = 0;
    mode = 0;
    versor[0] = 1.0;
    versor[1] = 1.0;
    versor[2] = 1.0;
    useVersor = false;
    rot = 0.0;
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
}

void admeshController::setMode(int m)
{
    mode = m;
}

void admeshController::pushHistory()
{
    history.add(objectList);
}

void admeshController::renewListView()
{
    listModel->removeRows(0,listModel->rowCount());
    for(QList<MeshObject*>::size_type i = 0; i < count;i++){
        addItemToView(objectList[i]);
    }
}

void admeshController::renewList()
{
    QList <MeshObject*> tmp = objectList;
    for(QList<MeshObject*>::size_type i = 0; i < count;i++){
        if(!tmp[i]->isActive()){    //inactive object is reused via same pointer
            tmp[i]->addReference();
        }else{
            tmp[i] = new MeshObject(*tmp[i]);
        }
    }
    objectList = tmp;
}

void admeshController::undo()
{
    objectList = history.undo();
    count = objectList.size();
    renewListView();
    for(QList<MeshObject*>::size_type i = 0; i < count;i++){
        objectList[i]->updateGeometry();
    }
}

void admeshController::redo()
{
    objectList = history.redo();
    count = objectList.size();
    renewListView();
    for(QList<MeshObject*>::size_type i = 0; i < count;i++){
        objectList[i]->updateGeometry();
    }
}

void admeshController::drawAll(QGLShaderProgram *program)
{
    if(mode == 0){
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        for(QList<MeshObject*>::size_type i = 0; i < count;i++){
            if(objectList[i]->isActive()){
                program->setUniformValue("color", GREEN);
                program->setUniformValue("badColor", RED);
            }else{
                program->setUniformValue("color", GREY);
                program->setUniformValue("badColor", GREY);
            }
            objectList[i]->drawGeometry(program);
        }
    }else if(mode == 1){
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        for(QList<MeshObject*>::size_type i = 0; i < count;i++){
            if(objectList[i]->isActive()){
                program->setUniformValue("color", BLACK);
                program->setUniformValue("badColor", BLACK);
            }else{
                program->setUniformValue("color", GREY);
                program->setUniformValue("badColor", GREY);
            }
            objectList[i]->drawGeometry(program);
        }
    }else if(mode == 2){
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        for(QList<MeshObject*>::size_type i = 0; i < count;i++){
            if(objectList[i]->isActive()){
                program->setUniformValue("color", BLACK);
                program->setUniformValue("badColor", BLACK);
            }else{
                program->setUniformValue("color", GREY);
                program->setUniformValue("badColor", GREY);
            }
            objectList[i]->drawGeometry(program);
        }
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glEnable( GL_POLYGON_OFFSET_FILL );
        glPolygonOffset( 1, 1 );
        for(QList<MeshObject*>::size_type i = 0; i < count;i++){
            if(objectList[i]->isActive()){
                program->setUniformValue("color", GREEN);
                program->setUniformValue("badColor", RED);
            }else{
                program->setUniformValue("color", GREY);
                program->setUniformValue("badColor", GREY);
            }
            objectList[i]->drawGeometry(program);
        }
    }
}

void admeshController::drawPicking(QGLShaderProgram *program)
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    for(QList<MeshObject*>::size_type i = 0; i < count;i++){
        program->setUniformValue("color", QVector3D((float)i/255,(float)i/255,(float)i/255));
        objectList[i]->drawGeometry(program);
    }
}

int admeshController::selectedCount()
{
    int selected = 0;
    for(QList<MeshObject*>::size_type i = 0; i < count;i++){
        if(objectList[i]->isActive()) selected++;
    }
    return selected;
}

void admeshController::setActiveByIndex(GLuint id)
{
    if(id<255 && count > 1) {
        objectList[id]->toggleActive();
        listView->selectionModel()->select( listModel->index(id), QItemSelectionModel::Toggle );
    }
}

void admeshController::setAllActive()
{
    for(QList<MeshObject*>::size_type i = 0; i < count;i++){
        objectList[i]->setActive();
    }
    listView->selectAll();
}

void admeshController::setAllInactive()
{
    if(count > 1){
        for(QList<MeshObject*>::size_type i = 0; i < count;i++){
            objectList[i]->setInactive();
        }
    }
    listView->clearSelection();
}

void admeshController::setAllInverseActive()
{
    if(count > 1){
        for(QList<MeshObject*>::size_type i = 0; i < count;i++){
            objectList[i]->toggleActive();
            listView->selectionModel()->select( listModel->index(i), QItemSelectionModel::Toggle );
        }
    }
}

void admeshController::setDrawColor(QVector3D col, QVector3D badCol){
    color = col;
    badColor = badCol;
}

void admeshController::addUIItems(QLabel *l,QListView *v)
{
    statusBar = l;
    listView = v;
    listModel = new QStringListModel();
    listView->setModel(listModel);
    connect(listView,SIGNAL(pressed(QModelIndex)),this,SLOT(handleSelectionChanged(QModelIndex)));
}

void admeshController::handleSelectionChanged(QModelIndex index)
{
    if(QApplication::mouseButtons() == Qt::LeftButton){
        if(index.row() < count) objectList[index.row()]->toggleActive();
    }
}

void admeshController::addItemToView(MeshObject* item){
    QFileInfo fi=item->getName();
    listModel->insertRow(listModel->rowCount());
    QModelIndex index = listModel->index(listModel->rowCount()-1);
    listModel->setData(index, fi.fileName());
    if(item->isActive()) listView->selectionModel()->select( index, QItemSelectionModel::Select );
}

QString admeshController::getInfo()
{
    QString text = "";
    float minx, miny, minz, maxx, maxy, maxz, num_facets, deg_facets, edges_fixed, facets_removed, facet_sadded, facets_reversed, backward, normals_fixed, volume;
    minx = miny = minz = maxx = maxy = maxz = num_facets = deg_facets = edges_fixed = facets_removed = facet_sadded = facets_reversed = backward = normals_fixed = volume = 0;
    int objects = 0;
    bool initialized = false;
    float* arr;
    for(QList<MeshObject*>::size_type i = 0; i < count;i++){
        if(objectList[i]->isActive()){
            arr = objectList[i]->getInfo();
            if(!initialized){
                minx = arr[0];
                miny = arr[1];
                minz = arr[2];
                maxx = arr[3];
                maxy = arr[4];
                maxz = arr[5];
                num_facets = arr[6];
                deg_facets = arr[7];
                edges_fixed = arr[8];
                facets_removed = arr[9];
                facet_sadded = arr[10];
                facets_reversed = arr[11];
                backward = arr[12];
                normals_fixed = arr[13];
                volume = arr[14];
                initialized = true;
            }else{
                if(arr[0]<minx)minx = arr[0];
                if(arr[1]<miny)miny = arr[1];
                if(arr[2]<minz)minz = arr[2];
                if(arr[3]>maxx)maxx = arr[3];
                if(arr[4]>maxy)maxy = arr[4];
                if(arr[5]>maxz)maxz = arr[5];
                num_facets += arr[6];
                deg_facets += arr[7];
                edges_fixed += arr[8];
                facets_removed += arr[9];
                facet_sadded += arr[10];
                facets_reversed += arr[11];
                backward += arr[12];
                normals_fixed += arr[13];
                volume += arr[14];
            }
            objects++;
            delete []arr;
        }
    }
    if(initialized){
        QTextStream(&text) << _("Objects selected:      ") << objects << endl <<
                              _("Min X:      ") << minx << endl <<
                              _("Min Y:      ") << miny << endl <<
                              _("Min Z:      ") << minz << endl <<
                              _("Max X:      ") << maxx << endl <<
                              _("Max Y:      ") << maxy << endl <<
                              _("Max Z:      ") << maxz << endl <<
                              _("Number of facets:      ") << num_facets << endl <<
                              _("Degenerate facets:     ") << deg_facets << endl <<
                              _("Edges fixed:           ") << edges_fixed << endl <<
                              _("Facets removed:        ") << facets_removed << endl <<
                              _("Facets added:          ") << facet_sadded << endl <<
                              _("Facets reversed:       ") << facets_reversed << endl <<
                              _("Backwards edges:       ") << backward << endl <<
                              _("Normals fixed:         ") << normals_fixed << endl <<
                              _("Total volume:                ") << volume << endl <<
                              endl;
    }
    return text;
}

void admeshController::openSTL()
{
    QString fileName = QFileDialog::getOpenFileName((QWidget*)parent(), _("Open STL"), "/", _("STL (*.stl)"));
    if(!fileName.isEmpty()){
        MeshObject* tmp = new MeshObject;
        if(!tmp->loadGeometry(fileName)){
            QString msg;
            QTextStream(&msg) << _("File ") << fileName << _(" could not be opened.\n");
            QMessageBox::critical(NULL, _("Error"), msg);
            return;
        }else{
            renewList();
            objectList.push_back(tmp);
            objectList.back()->setActive();
            addItemToView(objectList.back());
            pushHistory();
        }
        count++;
        if(selectedCount()>0)statusBar->setText(_("Status: File opened"));
        reCalculatePosition();
        reDrawSignal();
        enableEdit(true);
    }
}


void admeshController::openSTLbyName(const char* filename)
{
    QString fileName(filename);
    MeshObject* tmp = new MeshObject;
    if(!tmp->loadGeometry(fileName)){
        QString msg;
        QTextStream(&msg) << _("File ") << fileName << _(" could not be opened.\n");
        QMessageBox::critical(NULL, _("Error"), msg);        
        return;
    }else{
        objectList.push_back(tmp);
        objectList.back()->setActive();
        addItemToView(objectList.back());
        history.add(objectList);
    }
    count++;
    if(selectedCount()>0)statusBar->setText(_("Status: File(s) opened"));
    reCalculatePosition();
    reDrawSignal();
    enableEdit(true);
}

void admeshController::closeSTL()
{
    renewList();
    pushHistory();
    for(QList<MeshObject*>::size_type i = 0; i < count;i++){
        if(objectList[i]->isActive()){
            objectList.erase(objectList.begin() + i);
            --count;
            --i;
        }
    }

    if(count == 1) objectList[0]->setActive();
    else if(count == 0) enableEdit(false);
    renewListView();
}

void admeshController::saveAs()
{
    if(selectedCount()!=1){
        QString msg;
        QTextStream(&msg) << _("Select only one file to save.\n");
        QMessageBox::warning(NULL, _("Warning"), msg);
        return;
    }
    QString filter="STL_ascii (*.stl)";
    QString fileName=QFileDialog::getSaveFileName((QWidget*)parent(), _("Save as"), "/", _("STL_ascii (*.stl);;STL_binary (*.stl)"), &filter);
    if(!fileName.isEmpty()){
        fileName=fileName.section(".",0,0);
        fileName+=".stl";
        if(filter == "STL_ascii (*.stl)"){
            for(QList<MeshObject*>::size_type i = 0; i < count;i++){
                 if(objectList[i]->isActive()) {
                     objectList[i]->saveAs(fileName, 1);
                     statusBar->setText(_("Status: File saved as ASCII STL"));
                 }
            }
        }else if(filter == "STL_binary (*.stl)"){
            for(QList<MeshObject*>::size_type i = 0; i < count;i++){
                 if(objectList[i]->isActive()){
                     objectList[i]->saveAs(fileName, 2);
                     statusBar->setText(_("Status: File saved as binary STL"));
                 }
            }
        }
    }
}

void admeshController::saveObject(MeshObject* object)
{
    if(!object->isSaved() && object->hasValidName()){ // current mesh is saveable
        object->save();
        statusBar->setText(_("Status: File saved"));
    }else if(!object->isSaved()){ // current mesh has no valid name stored (eg. merged one)
        QString filter="STL_ascii (*.stl)";
        QString fileName=QFileDialog::getSaveFileName((QWidget*)parent(), _("Save as"), "/", _("STL_ascii (*.stl);;STL_binary (*.stl)"), &filter);
        if(!fileName.isEmpty()){
            fileName=fileName.section(".",0,0);
            fileName+=".stl";
            if(filter == "STL_ascii (*.stl)"){
               object->saveAs(fileName, 1);
               statusBar->setText(_("Status: File saved as ASCII STl"));
            }else if(filter == "STL_binary (*.stl)"){
               object->saveAs(fileName, 2);
               statusBar->setText(_("Status: File saved as ASCII STl"));
            }
        }
    }
}

void admeshController::save()
{
    for(QList<MeshObject*>::size_type i = 0; i < count;i++){
        if(objectList[i]->isActive()){
             saveObject(objectList[i]);
        }
    }
}

bool admeshController::saveOnClose()
{
    QMessageBox msgBox((QWidget*)parent());
    msgBox.setText(_("File has been modified."));
    msgBox.setInformativeText(_("Do you want to save changes?"));
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    for(QList<MeshObject*>::size_type i = 0; i < count;i++){
        if(!objectList[i]->isSaved()){
            QString msg;
            QTextStream(&msg) << _("File ") << objectList[i]->getName() << _(" has been modified.");
            msgBox.setText(msg);
            int ret = msgBox.exec();
            switch (ret) {
               case QMessageBox::Save:
                   saveObject(objectList[i]);
                   break;
               case QMessageBox::Cancel:
                   return false;
               default:
                   break;
             }
        }
    }
    return true;
}

void admeshController::exportSTL()
{
    if(selectedCount()!=1){
        QString msg;
        QTextStream(&msg) << _("Select only one file to export.\n");
        QMessageBox::warning(NULL, _("Warning"), msg);
        return;
    }
    QString filter="OBJ (*.obj)";
    QString fileName=QFileDialog::getSaveFileName((QWidget*)parent(), _("Export as"), "/", _("OBJ (*.obj);;OFF (*.off);;DXF (*.dxf);;VRML (*.vrml)"), &filter);
    if(!fileName.isEmpty()){
        fileName=fileName.section(".",0,0);
        if(filter == "OBJ (*.obj)"){
            fileName+=".obj";
            for(QList<MeshObject*>::size_type i = 0; i < count;i++){
                 if(objectList[i]->isActive()) {
                     objectList[i]->exportSTL(fileName, 1);
                     if(selectedCount()>0)statusBar->setText(_("Status: File exported to OBJ format"));
                 }
            }
        }else if(filter == "OFF (*.off)"){
            fileName+=".off";
            for(QList<MeshObject*>::size_type i = 0; i < count;i++){
                 if(objectList[i]->isActive()) {
                     objectList[i]->exportSTL(fileName, 2);
                     if(selectedCount()>0)statusBar->setText(_("Status: File exported to OFF format"));
                 }
            }
        }else if(filter == "DXF (*.dxf)"){
            fileName+=".dxf";
            for(QList<MeshObject*>::size_type i = 0; i < count;i++){
                 if(objectList[i]->isActive()) {
                     objectList[i]->exportSTL(fileName, 3);
                     if(selectedCount()>0)statusBar->setText(_("Status: File exported to DXF format"));
                 }
            }
        }else if(filter == "VRML (*.vrml)"){
            fileName+=".vrml";
            for(QList<MeshObject*>::size_type i = 0; i < count;i++){
                 if(objectList[i]->isActive()) {
                     objectList[i]->exportSTL(fileName, 4);
                     if(selectedCount()>0)statusBar->setText(_("Status: File exported to VRML format"));
                 }
            }
        }
    }
}

QVector3D admeshController::getMinPosition()
{
    if(objectList.back()) return objectList.back()->getMin();
    else return QVector3D(0.0,0.0,0.0);
}

void admeshController::setVersorX(double factor)
{
    if(useVersor){
        versor[0]=factor;
    }else{
        versor[0]=versor[1]=versor[2]=factor;
        scaleSignal(factor);
    }
}

void admeshController::setVersorY(double factor)
{
    if(useVersor){
        versor[1]=factor;
    }else{
        versor[0]=versor[1]=versor[2]=factor;
        scaleSignal(factor);
    }
}

void admeshController::setVersorZ(double factor)
{
    if(useVersor){
        versor[2]=factor;
    }else{
        versor[0]=versor[1]=versor[2]=factor;
        scaleSignal(factor);
    }
}

void admeshController::setVersor()
{
    useVersor = !useVersor;
    if(!useVersor)scaleSignal(versor[0]); //set all fields to X value
}

void admeshController::scale()
{
    if(versor[0] != 0.0 || versor[1] != 0.0 || versor[2] != 0.0){
        renewList();
        for(QList<MeshObject*>::size_type i = 0; i < count;i++){
            if(objectList[i]->isActive())objectList[i]->scale(versor);
        }
        if(selectedCount()>0)statusBar->setText(_("Status: mesh(es) scaled"));
        pushHistory();
    }
}

void admeshController::mirrorXY()
{
    renewList();
    for(QList<MeshObject*>::size_type i = 0; i < count;i++){
        if(objectList[i]->isActive())objectList[i]->mirrorXY();
    }
    if(selectedCount()>0)statusBar->setText(_("Status: mesh(es) mirrored along XY plane"));
    pushHistory();
}

void admeshController::mirrorYZ()
{
    renewList();
    for(QList<MeshObject*>::size_type i = 0; i < count;i++){
        if(objectList[i]->isActive())objectList[i]->mirrorYZ();
    }
    if(selectedCount()>0)statusBar->setText(_("Status: mesh(es) mirrored along YZ plane"));
    pushHistory();
}

void admeshController::mirrorXZ()
{
    renewList();
    for(QList<MeshObject*>::size_type i = 0; i < count;i++){
        if(objectList[i]->isActive())objectList[i]->mirrorXZ();
    }
    if(selectedCount()>0)statusBar->setText(_("Status: mesh(es) mirrored along XZ plane"));
    pushHistory();
}

void admeshController::setRot(double angle)
{
    rot = (float)angle;
}

void admeshController::rotateX()
{
    if(rot != 0.0){
        renewList();
        for(QList<MeshObject*>::size_type i = 0; i < count;i++){
            if(objectList[i]->isActive())objectList[i]->rotateX(rot);
        }
        if(selectedCount()>0)statusBar->setText(_("Status: mesh(es) rotated along X axis"));
        pushHistory();
    }
}

void admeshController::rotateY()
{
    if(rot != 0.0){
        renewList();
        for(QList<MeshObject*>::size_type i = 0; i < count;i++){
            if(objectList[i]->isActive())objectList[i]->rotateY(rot);
        }
        if(selectedCount()>0)statusBar->setText(_("Status: mesh(es) rotated along Y axis"));
        pushHistory();
    }
}

void admeshController::rotateZ()
{
    if(rot != 0.0){
        renewList();
        for(QList<MeshObject*>::size_type i = 0; i < count;i++){
            if(objectList[i]->isActive())objectList[i]->rotateZ(rot);
        }
        if(selectedCount()>0)statusBar->setText(_("Status: mesh(es) rotated along Z axis"));
        pushHistory();
    }
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
    if(!rel_translate || x_translate != 0.0 || y_translate != 0.0 || z_translate != 0.0){
        renewList();
        for(QList<MeshObject*>::size_type i = 0; i < count;i++){
            if(objectList[i]->isActive())objectList[i]->translate(rel_translate, x_translate, y_translate, z_translate);
        }
        if(selectedCount()>0 && rel_translate)statusBar->setText(_("Status: mesh(es) translated relatively to position"));
        else if(selectedCount()>0)statusBar->setText(_("Status: mesh(es) translated to origin"));
        pushHistory();
    }
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
    renewList();
    for(QList<MeshObject*>::size_type i = 0; i < count;i++){
        if(objectList[i]->isActive())objectList[i]->repair(fixall_flag,
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
    if(selectedCount()>0)statusBar->setText(_("Status: mesh(es) repaired"));
    pushHistory();
}

void stl_merge(stl_file *stl, stl_file *stl_to_merge) {
    int num_facets_so_far;
    if (stl->error) return;
    num_facets_so_far = stl->stats.number_of_facets;
    stl->stats.type=stl_to_merge->stats.type;
    stl->stats.number_of_facets+=stl_to_merge->stats.number_of_facets;
    stl_reallocate(stl);
    for(int i=0; i<stl_to_merge->stats.number_of_facets;i++){
        stl->facet_start[num_facets_so_far+i] = stl_to_merge->facet_start[i];
        stl->neighbors_start[num_facets_so_far+i] = stl_to_merge->neighbors_start[i];
    }
    stl->stats.max.x = (stl->stats.max.x > stl_to_merge->stats.max.x) ? stl->stats.max.x : stl_to_merge->stats.max.x;
    stl->stats.max.y = (stl->stats.max.y > stl_to_merge->stats.max.y) ? stl->stats.max.y : stl_to_merge->stats.max.y;
    stl->stats.max.z = (stl->stats.max.z > stl_to_merge->stats.max.z) ? stl->stats.max.z : stl_to_merge->stats.max.z;
    stl->stats.min.x = (stl->stats.min.x < stl_to_merge->stats.min.x) ? stl->stats.min.x : stl_to_merge->stats.min.x;
    stl->stats.min.y = (stl->stats.min.y < stl_to_merge->stats.min.y) ? stl->stats.min.y : stl_to_merge->stats.min.y;
    stl->stats.min.z = (stl->stats.min.z < stl_to_merge->stats.min.z) ? stl->stats.min.z : stl_to_merge->stats.min.z;
    stl->stats.size.x = stl->stats.max.x - stl->stats.min.x;
    stl->stats.size.y = stl->stats.max.y - stl->stats.min.y;
    stl->stats.size.z = stl->stats.max.z - stl->stats.min.z;
    stl->stats.bounding_diameter = sqrt(
                                     stl->stats.size.x * stl->stats.size.x +
                                     stl->stats.size.y * stl->stats.size.y +
                                     stl->stats.size.z * stl->stats.size.z
                                     );
}

void admeshController::merge()
{
    if(selectedCount()<2){
        QString msg;
        QTextStream(&msg) << _("Not enough files to merge selected.\n");
        QMessageBox::warning(NULL, _("Warning"), msg);
        return;
    }
    MeshObject *mergedMesh;
    int next = 0;
    for(QList<MeshObject*>::size_type i = 0; i < count;i++){
        if(objectList[i]->isActive()){
            mergedMesh = new MeshObject(*objectList[i]);
            objectList.erase(objectList.begin() + i);
            --count;
            next = i;
            --i;
            break;
        }
    }
    for(QList<MeshObject*>::size_type i = next; i < count;i++){
        if(objectList[i]->isActive()){
            stl_merge(mergedMesh->getStlPointer(), objectList[i]->getStlPointer());
            objectList.erase(objectList.begin() + i);
            --count;
            --i;
        }
    }
    stl_calculate_volume(mergedMesh->getStlPointer());
    mergedMesh->resetFilename();
    mergedMesh->updateGeometry();
    objectList.push_back(mergedMesh);
    count++;
    renewListView();
    statusBar->setText(_("Status: meshes merged"));
    pushHistory();
}
