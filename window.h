#ifndef WINDOW_H
#define WINDOW_H

#include <QtWidgets>
#include <QWidget>
#include <QSlider>

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

protected:
    void keyPressEvent(QKeyEvent *event);

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
