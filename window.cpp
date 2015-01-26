#include "window.h"
#include "ui_window.h"
#include "renderingwidget.h"

Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);

    connect(ui->renderingWidget, SIGNAL(xRotationChanged(int)), ui->xRotSlider, SLOT(setValue(int)));
    connect(ui->renderingWidget, SIGNAL(yRotationChanged(int)), ui->yRotSlider, SLOT(setValue(int)));
    ui->showButton->hide();

    controller = new admeshController;
    ui->renderingWidget->setController(controller);
    connect(controller, SIGNAL(reDrawSignal()), ui->renderingWidget, SLOT(reDraw()));
    connect(controller, SIGNAL(reCalculatePosition()), ui->renderingWidget, SLOT(reCalculatePosition()));
    addActions();
    addMenus();

    connect(ui->scaleBox, SIGNAL(valueChanged(double)), controller, SLOT(setScale(double)));
    connect(ui->scaleButton, SIGNAL(clicked()), controller, SLOT(scale()));
    connect(ui->mirrorxyButton, SIGNAL(clicked()), controller, SLOT(mirrorXY()));
    connect(ui->mirroryzButton, SIGNAL(clicked()), controller, SLOT(mirrorYZ()));
    connect(ui->mirrorxzButton, SIGNAL(clicked()), controller, SLOT(mirrorXZ()));
    connect(ui->rotateXBox, SIGNAL(valueChanged(double)), controller, SLOT(setXRot(double)));
    connect(ui->rotateXButton, SIGNAL(clicked()), controller, SLOT(rotateX()));
    connect(ui->rotateYBox, SIGNAL(valueChanged(double)), controller, SLOT(setYRot(double)));
    connect(ui->rotateYButton, SIGNAL(clicked()), controller, SLOT(rotateY()));
    connect(ui->rotateZBox, SIGNAL(valueChanged(double)), controller, SLOT(setZRot(double)));
    connect(ui->rotateZButton, SIGNAL(clicked()), controller, SLOT(rotateZ()));
    connect(ui->translateXBox, SIGNAL(valueChanged(double)), controller, SLOT(setXTranslate(double)));
    connect(ui->translateYBox, SIGNAL(valueChanged(double)), controller, SLOT(setYTranslate(double)));
    connect(ui->translateZBox, SIGNAL(valueChanged(double)), controller, SLOT(setZTranslate(double)));
    connect(ui->translateRelBox, SIGNAL(stateChanged(int)), controller, SLOT(setRelativeTranslate()));
    connect(ui->translateButton, SIGNAL(clicked()), controller, SLOT(translate()));

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
    connect(ui->reverseBox, SIGNAL(stateChanged(int)), controller, SLOT(setReverseAllFlag()));
    connect(ui->fixAllBox, SIGNAL(stateChanged(int)), controller, SLOT(setFixAllFlag()));
    connect(ui->repairButton, SIGNAL(clicked()), controller, SLOT(repair()));
}

Window::~Window()
{
    delete ui;
    delete controller;
}

void Window::addActions(){
    openAct = new QAction(_("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(_("Open STL file"));
    connect(openAct, SIGNAL(triggered()), controller, SLOT(openSTL()));

    saveAsAct = new QAction(_("&Save as"), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(_("Save as ASCII or binary STL file"));
    connect(saveAsAct, SIGNAL(triggered()), controller, SLOT(saveAs()));

    exportAct = new QAction(_("&Export"), this);
    exportAct->setStatusTip(_("Export as OBJ, OFF, DXF or VRML"));
    connect(exportAct, SIGNAL(triggered()), controller, SLOT(exportSTL()));

    axesAct = new QAction(_("&Axes"), this);
    axesAct->setStatusTip(_("Show or hide axes"));
    axesAct->setCheckable(true);
    axesAct->setChecked(true);
    axesAct->setShortcut(AXES_SHORTCUT);
    connect(axesAct, SIGNAL(triggered()), ui->renderingWidget, SLOT(toggleAxes()));

    modeAct = new QAction(_("&Solid Mode"), this);
    modeAct->setStatusTip(_("Toggle solid or wireframe"));
    modeAct->setCheckable(true);
    modeAct->setChecked(true);
    modeAct->setShortcut(SOLID_SHORTCUT);
    connect(modeAct, SIGNAL(triggered()), ui->renderingWidget, SLOT(toggleMode()));
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
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(exportAct);
    fileMenu->addSeparator();
    editMenu = menu_bar->addMenu(_("&Edit"));
    viewMenu = menu_bar->addMenu(_("&View"));
    viewMenu->addAction(axesAct);
    viewMenu->addAction(modeAct);
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
    }
    else if(e->key() == SOLID_SHORTCUT){
        ui->renderingWidget->toggleMode();
        modeAct->toggle();
    }
    else
        QWidget::keyPressEvent(e);
}
