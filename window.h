#ifndef WINDOW_H
#define WINDOW_H

#include <QtWidgets>
#include <QWidget>
#include <QSlider>
#include "admeshcontroller.h"

namespace Ui {
class Window;
}

/*!
 * \brief Main application window.
 *
 * Contains main rendering widget, menu, buttons layout.
 */
class Window : public QWidget
{
    Q_OBJECT

public:
    /*!
     * \brief Constructor.
     */
    explicit Window(QWidget *parent = 0);

    /*!
     * \brief Destructor.
     */
    ~Window();

public slots:
    /*!
     * \brief Sends command to controller to open file.
     * \param filename Filename given.
     */
    void openByFilename(const char* filename);

protected:
    /*!
     * \brief Reimplemented method. Handles key pressed.
     */
    void keyPressEvent(QKeyEvent *event);

    admeshController *controller;    ///< Main ADMeshController

private:
    Ui::Window *ui;                 ///< Holds user interface.
    void addActions();              ///< Creates menu actions.
    void addMenus();                ///< Creates menu.
    QMenu *fileMenu;                ///< File menu.
    QMenu *editMenu;                ///< Editation menu.
    QMenu *viewMenu;                ///< View menu.
    QAction *openAct;               ///< Open file action.
    QAction *saveAsAct;             ///< Save as file action.
    QAction *exportAct;             ///< Export file action.
    QAction *axesAct;               ///< Show axes action.
    QAction *modeAct;               ///< Toggle mode action.
};

#endif // WINDOW_H
