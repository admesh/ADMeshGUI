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

    addActions();
    addMenus();
}

Window::~Window()
{
    delete ui;
}

void Window::addActions(){
    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open STL file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    axesAct = new QAction(tr("&Axes"), this);
    axesAct->setStatusTip(tr("Show or hide axes"));
    axesAct->setCheckable(true);
    axesAct->setChecked(true);
    connect(axesAct, SIGNAL(triggered()), ui->renderingWidget, SLOT(toggleAxes()));

    modeAct = new QAction(tr("&Solid Mode"), this);
    modeAct->setStatusTip(tr("Toggle solid or wireframe"));
    modeAct->setCheckable(true);
    modeAct->setChecked(true);
    connect(modeAct, SIGNAL(triggered()), ui->renderingWidget, SLOT(toggleMode()));
}

void Window::addMenus(){
    QMenuBar *menu_bar = new QMenuBar();
    menu_bar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->menuLayout->addWidget(menu_bar);
    menu_bar->setContentsMargins(0,0,0,0);

    fileMenu = menu_bar->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    editMenu = menu_bar->addMenu(tr("&Edit"));
    viewMenu = menu_bar->addMenu(tr("&View"));
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
