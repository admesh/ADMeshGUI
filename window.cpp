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
    addActions();
    addMenus();
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

    axesAct = new QAction(_("&Axes"), this);
    axesAct->setStatusTip(_("Show or hide axes"));
    axesAct->setCheckable(true);
    axesAct->setChecked(true);
    connect(axesAct, SIGNAL(triggered()), ui->renderingWidget, SLOT(toggleAxes()));

    modeAct = new QAction(_("&Solid Mode"), this);
    modeAct->setStatusTip(_("Toggle solid or wireframe"));
    modeAct->setCheckable(true);
    modeAct->setChecked(true);
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
    fileMenu->addSeparator();
    editMenu = menu_bar->addMenu(_("&Edit"));
    viewMenu = menu_bar->addMenu(_("&View"));
    viewMenu->addAction(axesAct);
    viewMenu->addAction(modeAct);
}

void Window::open()
 {

 }

void Window::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}
