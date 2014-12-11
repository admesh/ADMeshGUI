#ifndef WINDOW_H
#define WINDOW_H

#include <QtWidgets>
#include <QWidget>
#include <QSlider>
#include "admeshcontroller.h"

namespace Ui {
class Window;
}

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();

public slots:
    void open();
    void openByFilename(const char* filename);

protected:
    void keyPressEvent(QKeyEvent *event);
    admeshController *controller;

private:
    Ui::Window *ui;
    void addActions();
    void addMenus();
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;

    QAction *openAct;
    QAction *axesAct;
    QAction *modeAct;
};

#endif // WINDOW_H
