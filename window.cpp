#include "window.h"
#include "ui_window.h"
#include "renderingwidget.h"

Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);

    ui->showButton->hide();
    ui->showButtonLeft->hide();
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    controller = new admeshController(this);
    controller->addUIItems(ui->statusBar, ui->listView);
    ui->renderingWidget->setController(controller);
    ui->buttonWidget->setEnabled(false);
    connect(controller, SIGNAL(reDrawSignal()), ui->renderingWidget, SLOT(reDraw()));
    connect(controller, SIGNAL(reCalculatePosition()), ui->renderingWidget, SLOT(reCalculatePosition()));
    connect(controller, SIGNAL(enableEdit(bool)), ui->buttonWidget,SLOT(setEnabled(bool)));
    connect(controller, SIGNAL(scaleSignal(double)), ui->versorXBox,SLOT(setValue(double)));
    connect(controller, SIGNAL(scaleSignal(double)), ui->versorYBox,SLOT(setValue(double)));
    connect(controller, SIGNAL(scaleSignal(double)), ui->versorZBox,SLOT(setValue(double)));
    addActions();
    addMenus();
    addToolbars();

    connect(ui->versorXBox, SIGNAL(valueChanged(double)), controller, SLOT(setVersorX(double)));
    connect(ui->versorYBox, SIGNAL(valueChanged(double)), controller, SLOT(setVersorY(double)));
    connect(ui->versorZBox, SIGNAL(valueChanged(double)), controller, SLOT(setVersorZ(double)));
    connect(ui->useVersorBox, SIGNAL(stateChanged(int)), controller, SLOT(setVersor()));
    connect(ui->scaleButton, SIGNAL(clicked()), controller, SLOT(scale()));
    connect(ui->mirrorxyButton, SIGNAL(clicked()), controller, SLOT(mirrorXY()));
    connect(ui->mirroryzButton, SIGNAL(clicked()), controller, SLOT(mirrorYZ()));
    connect(ui->mirrorxzButton, SIGNAL(clicked()), controller, SLOT(mirrorXZ()));
    connect(ui->RotateBox, SIGNAL(valueChanged(double)), controller, SLOT(setRot(double)));
    connect(ui->rotateXButton, SIGNAL(clicked()), controller, SLOT(rotateX()));
    connect(ui->rotateYButton, SIGNAL(clicked()), controller, SLOT(rotateY()));
    connect(ui->rotateZButton, SIGNAL(clicked()), controller, SLOT(rotateZ()));
    connect(ui->translateXBox, SIGNAL(valueChanged(double)), controller, SLOT(setXTranslate(double)));
    connect(ui->translateYBox, SIGNAL(valueChanged(double)), controller, SLOT(setYTranslate(double)));
    connect(ui->translateZBox, SIGNAL(valueChanged(double)), controller, SLOT(setZTranslate(double)));
    connect(ui->translateRelBox, SIGNAL(stateChanged(int)), controller, SLOT(setRelativeTranslate()));
    connect(ui->translateButton, SIGNAL(clicked()), controller, SLOT(translate()));
    connect(ui->mergeButton, SIGNAL(clicked()), controller, SLOT(merge()));
    connect(ui->splitButton, SIGNAL(clicked()), controller, SLOT(split()));

    connect(ui->hideItemsButton, SIGNAL(clicked()), controller, SLOT(hide()));
    connect(ui->unhideItemsButton, SIGNAL(clicked()), controller, SLOT(unhide()));

    connect(ui->exactBox, SIGNAL(stateChanged(int)), controller, SLOT(setExactFlag()));
    connect(ui->toleranceBox, SIGNAL(stateChanged(int)), controller, SLOT(setToleranceFlag()));
    connect(ui->toleranceSpinBox, SIGNAL(valueChanged(double)), controller, SLOT(setTolerance(double)));
    connect(ui->incrementBox, SIGNAL(stateChanged(int)), controller, SLOT(setIncrementFlag()));
    connect(ui->incrementSpinBox, SIGNAL(valueChanged(double)), controller, SLOT(setIncrement(double)));
    connect(ui->nearbyBox, SIGNAL(stateChanged(int)), controller, SLOT(setNearbyFlag()));
    connect(ui->iterationsSpinBox, SIGNAL(valueChanged(int)), controller, SLOT(setIterations(int)));
    connect(ui->unconnectedBox, SIGNAL(stateChanged(int)), controller, SLOT(setRemoveUnconnectedFlag()));
    connect(ui->fillholesBox, SIGNAL(stateChanged(int)), controller, SLOT(setFillHolesFlag()));
    connect(ui->normalDirBox, SIGNAL(stateChanged(int)), controller, SLOT(setNormalDirFlag()));
    connect(ui->normalValBox, SIGNAL(stateChanged(int)), controller, SLOT(setNormalValFlag()));
    connect(ui->reverseButton, SIGNAL(clicked()), controller, SLOT(reverseAll()));
    connect(ui->fixAllBox, SIGNAL(stateChanged(int)), controller, SLOT(setFixAllFlag()));
    connect(ui->repairButton, SIGNAL(clicked()), controller, SLOT(repair()));

    readSettings();
}

Window::~Window()
{
    delete ui;
    delete controller;
}

void Window::addActions(){
    openAct = new QAction(_("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(_("Open STL file"));
    connect(openAct, SIGNAL(triggered()), controller, SLOT(openSTL()));

    saveAct = new QAction(_("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(_("Save in default STL format"));
    connect(saveAct, SIGNAL(triggered()), controller, SLOT(save()));

    saveAsAct = new QAction(_("&Save as..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(_("Save as ASCII or binary STL file"));
    connect(saveAsAct, SIGNAL(triggered()), controller, SLOT(saveAs()));

    exportAct = new QAction(_("&Export..."), this);
    exportAct->setShortcut(EXPORT_SHORTCUT);
    exportAct->setStatusTip(_("Export as OBJ, OFF, DXF or VRML"));
    connect(exportAct, SIGNAL(triggered()), controller, SLOT(exportSTL()));

    closeAct = new QAction(_("&Close"), this);
    closeAct->setShortcuts(QKeySequence::Close);
    closeAct->setStatusTip(_("Close selected files"));
    connect(closeAct, SIGNAL(triggered()), controller, SLOT(closeSTL()));

    quitAct = new QAction(_("&Quit"), this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(_("Quit application"));
    connect(quitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));


    axesAct = new QAction(_("&Axes"), this);
    axesAct->setStatusTip(_("Show or hide axes"));
    axesAct->setCheckable(true);
    axesAct->setChecked(true);
    axesAct->setShortcut(AXES_SHORTCUT);
    connect(axesAct, SIGNAL(triggered()), ui->renderingWidget, SLOT(toggleAxes()));

    gridAct = new QAction(_("&Grid"), this);
    gridAct->setStatusTip(_("Show or hide grid"));
    gridAct->setCheckable(true);
    gridAct->setChecked(false);
    gridAct->setShortcut(GRID_SHORTCUT);
    connect(gridAct, SIGNAL(triggered()), ui->renderingWidget, SLOT(toggleGrid()));

    solidAct = new QAction(_("&Solid Mode"), this);
    solidAct->setStatusTip(_("Show solid mesh"));
    solidAct->setCheckable(true);
    solidAct->setChecked(true);
    solidAct->setShortcut(SOLID_SHORTCUT);
    connect(solidAct, SIGNAL(triggered()), this, SLOT(setSolid()));

    wireframeAct = new QAction(_("&Wireframe Mode"), this);
    wireframeAct->setStatusTip(_("Show wireframe mesh"));
    wireframeAct->setCheckable(true);
    wireframeAct->setChecked(false);
    wireframeAct->setShortcut(WIREFRAME_SHORTCUT);
    connect(wireframeAct, SIGNAL(triggered()), this, SLOT(setWireframe()));

    solidwithedgesAct = new QAction(_("&Solid with edged Mode"), this);
    solidwithedgesAct->setStatusTip(_("Show solid mesh with edges"));
    solidwithedgesAct->setCheckable(true);
    solidwithedgesAct->setChecked(false);
    solidwithedgesAct->setShortcut(EDGES_SHORTCUT);
    connect(solidwithedgesAct, SIGNAL(triggered()), this, SLOT(setSolidWithEdges()));

    infoAct = new QAction(_("&Info"), this);
    infoAct->setStatusTip(_("Show or hide info"));
    infoAct->setCheckable(true);
    infoAct->setChecked(true);
    infoAct->setShortcut(INFO_SHORTCUT);
    connect(infoAct, SIGNAL(triggered()), ui->renderingWidget, SLOT(toggleInfo()));

    frontAct = new QAction(_("&Front view"), this);
    frontAct->setStatusTip(_("Set front view"));
    frontAct->setShortcut(FRONT_SHORTCUT);
    connect(frontAct, SIGNAL(triggered()), ui->renderingWidget, SLOT(setFrontView()));

    backAct = new QAction(_("&Back view"), this);
    backAct->setStatusTip(_("Set back view"));
    backAct->setShortcut(BACK_SHORTCUT);
    connect(backAct, SIGNAL(triggered()), ui->renderingWidget, SLOT(setBackView()));

    leftAct = new QAction(_("&Left view"), this);
    leftAct->setStatusTip(_("Set left view"));
    leftAct->setShortcut(LEFT_SHORTCUT);
    connect(leftAct, SIGNAL(triggered()), ui->renderingWidget, SLOT(setLeftView()));

    rightAct = new QAction(_("&Right view"), this);
    rightAct->setStatusTip(_("Set right view"));
    rightAct->setShortcut(RIGHT_SHORTCUT);
    connect(rightAct, SIGNAL(triggered()), ui->renderingWidget, SLOT(setRightView()));

    topAct = new QAction(_("&Top view"), this);
    topAct->setStatusTip(_("Set top view"));
    topAct->setShortcut(TOP_SHORTCUT);
    connect(topAct, SIGNAL(triggered()), ui->renderingWidget, SLOT(setTopView()));

    bottomAct = new QAction(_("&Bottom view"), this);
    bottomAct->setStatusTip(_("Set bottom view"));
    bottomAct->setShortcut(BOTTOM_SHORTCUT);
    connect(bottomAct, SIGNAL(triggered()), ui->renderingWidget, SLOT(setBottomView()));

    centerAct = new QAction(_("&To center"), this);
    centerAct->setStatusTip(_("Reset camera translation to zero"));
    centerAct->setShortcut(RESET_SHORTCUT);
    connect(centerAct, SIGNAL(triggered()), ui->renderingWidget, SLOT(centerPosition()));

    selectAllAct = new QAction(_("&Select all"), this);
    selectAllAct->setStatusTip(_("Make all objects in scene active"));
    selectAllAct->setShortcut(QKeySequence::SelectAll);
    connect(selectAllAct, SIGNAL(triggered()), controller, SLOT(setAllActive()));

    selectInverseAct = new QAction(_("&Select inverse"), this);
    selectInverseAct->setStatusTip(_("Active objects are set inactive and vice versa"));
    selectInverseAct->setShortcut(QKeySequence::Italic);
    connect(selectInverseAct, SIGNAL(triggered()), controller, SLOT(setAllInverseActive()));

    undoAct = new QAction(_("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(_("Undo last action"));
    connect(undoAct, SIGNAL(triggered()), controller, SLOT(undo()));

    redoAct = new QAction(_("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(_("Redo last action"));
    connect(redoAct, SIGNAL(triggered()), controller, SLOT(redo()));
}

void Window::addMenus(){
    QMenuBar *menu_bar = new QMenuBar(0);
    menu_bar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    menu_bar->show();
    menu_bar->setNativeMenuBar (false);
    ui->menuLayout->addWidget(menu_bar);
    menu_bar->setContentsMargins(0,0,0,0);
    fileMenu = menu_bar->addMenu(_("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(exportAct);
    fileMenu->addSeparator();
    fileMenu->addAction(closeAct);
    fileMenu->addAction(quitAct);
    editMenu = menu_bar->addMenu(_("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addSeparator();
    editMenu->addAction(selectAllAct);
    editMenu->addAction(selectInverseAct);
    viewMenu = menu_bar->addMenu(_("&View"));
    viewMenu->addAction(infoAct);
    viewMenu->addAction(axesAct);
    viewMenu->addAction(gridAct);
    viewMenu->addSeparator();
    viewMenu->addAction(solidAct);
    viewMenu->addAction(wireframeAct);
    viewMenu->addAction(solidwithedgesAct);
    viewMenu->addSeparator();
    viewMenu->addAction(centerAct);
    viewMenu->addSeparator();
    viewMenu->addAction(frontAct);
    viewMenu->addAction(backAct);
    viewMenu->addAction(leftAct);
    viewMenu->addAction(rightAct);
    viewMenu->addAction(topAct);
    viewMenu->addAction(bottomAct);
}

void Window::addToolbars()
{
    QToolBar *toolBar = new QToolBar(0);
    toolBar->show();
    toolBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->toolBarLayout->addWidget(toolBar);

    openButton = new QToolButton();
    openButton->setDefaultAction(openAct);
    openButton->setIcon(QIcon::fromTheme("document-new", QIcon("://Resources/open.svg")));
    openButton->setFixedSize(35, 30);
    toolBar->addWidget(openButton);

    saveButton = new QToolButton();
    saveButton->setDefaultAction(saveAct);
    saveButton->setIcon(QIcon::fromTheme("document-save", QIcon("://Resources/save.svg")));
    saveButton->setFixedSize(35, 30);
    toolBar->addWidget(saveButton);

    undoButton = new QToolButton();
    undoButton->setDefaultAction(undoAct);
    undoButton->setIcon(QIcon::fromTheme("edit-undo", QIcon("://Resources/undo.svg")));
    undoButton->setFixedSize(35, 30);
    toolBar->addWidget(undoButton);

    redoButton = new QToolButton();
    redoButton->setDefaultAction(redoAct);
    redoButton->setIcon(QIcon::fromTheme("edit-redo", QIcon("://Resources/redo.svg")));
    redoButton->setFixedSize(35, 30);
    toolBar->addWidget(redoButton);

    closeButton = new QToolButton();
    closeButton->setDefaultAction(closeAct);
    closeButton->setIcon(QIcon::fromTheme("edit-delete", QIcon("://Resources/close.svg")));
    closeButton->setFixedSize(35, 30);
    toolBar->addWidget(closeButton);
}

void Window::openByFilename(const char* filename){
    controller->openSTLbyName(filename);
}

void Window::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        close();
    else if(e->key() == AXES_SHORTCUT){
        ui->renderingWidget->toggleAxes();
        axesAct->toggle();
    } else if(e->key() == Qt::Key_Shift){
        ui->renderingWidget->toggleShift();
    }else {
        QWidget::keyPressEvent(e);
    }
}


void Window::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Shift){
        ui->renderingWidget->toggleShift();
    } else {
        QWidget::keyReleaseEvent(event);
    }
}

void Window::closeEvent(QCloseEvent *event)
{
    if(controller->saveOnClose()){
        writeSettings();
        event->accept();
    }else{
        event->ignore();
    }

}

void Window::setSolid()
{
    controller->setMode(0);
    solidAct->setChecked(true);
    wireframeAct->setChecked(false);
    solidwithedgesAct->setChecked(false);
}

void Window::setWireframe()
{
    controller->setMode(1);
    solidAct->setChecked(false);
    wireframeAct->setChecked(true);
    solidwithedgesAct->setChecked(false);
}

void Window::setSolidWithEdges()
{
    controller->setMode(2);
    solidAct->setChecked(false);
    wireframeAct->setChecked(false);
    solidwithedgesAct->setChecked(true);
}

void Window::writeSettings()
{
    QSettings settings;
    if(this->isMaximized())settings.setValue("maximized",true);
    else settings.setValue("maximized",false);
    settings.setValue("width", ui->renderingWidget->width());
    settings.setValue("height", ui->renderingWidget->height());
    settings.setValue("mode", 2);
    controller->writeSettings();
    ui->renderingWidget->writeSettings();
}

void Window::readSettings()
{
    QSettings settings;
    if(settings.value("maximized",false).toBool())this->showMaximized();
    int mode = settings.value("rendermode",0).toInt();
    switch(mode){
        case 0:
            setSolid();
            break;
        case 1:
            setWireframe();
            break;
        case 2:
            setSolidWithEdges();
            break;
    }
    if(!settings.value("axes", true).toBool()) axesAct->trigger();
    if(settings.value("grid", true).toBool()) gridAct->trigger();
    if(!settings.value("info", true).toBool()) infoAct->trigger();
}
