// (c) 2015 David Vyvlečka, AGPLv3

#ifndef WINDOW_H
#define WINDOW_H

#include <QtWidgets>
#include <QWidget>
#include <QSlider>
#include "admeshcontroller.h"
#include "propertiesdialog.h"

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

    /*!
     * \brief Sets solid mode.
     */
    void setSolid();

    /*!
     * \brief Sets wireframe mode.
     */
    void setWireframe();

    /*!
     * \brief Sets solid mode with thick edges.
     */
    void setSolidWithEdges();

    /*!
     * \brief Toggle color scheme. Light and dark scheme supported.
     */
    void toggleColorScheme();

    /*!
     * \brief Set selected color scheme.
     */
    void setColorScheme();

    /*!
     * \brief Toggle inverted mouse.
     */
    void toggleMouseInvert();

    /*!
     * \brief Initialize and exec properties dialog.
     */
    void initProperties();

protected:
    /*!
     * \brief Reimplemented method. Handles key press.
     */
    void keyPressEvent(QKeyEvent *event);

    /*!
     * \brief Reimplemented method. Handles key release.
     */
    void keyReleaseEvent(QKeyEvent *event);

    /*!
     * \brief Reimplemented method. Handles closed window (application exit).
     */
    void closeEvent(QCloseEvent *event);

    void dropEvent(QDropEvent *event);

    void dragEnterEvent(QDragEnterEvent *event);

    admeshController *controller;    ///< Main ADMeshController

private:
    /*!
     * \brief Write settings to config.
     */
    void writeSettings();
    /*!
     * \brief Read settings from config.
     */
    void readSettings();
    int scheme;                     ///< Color scheme selected;
    Ui::Window *ui;                 ///< Holds user interface.
    void addActions();              ///< Creates menu actions.
    void addMenus();                ///< Creates menu.
    void addToolbars();             ///< Creates toolbar.
    QMenu *fileMenu;                ///< File menu.
    QMenu *editMenu;                ///< Editation menu.
    QMenu *viewMenu;                ///< View menu.
    QAction *openAct;               ///< Open file action.
    QAction *saveAct;               ///< Save file action.
    QAction *saveAsAct;             ///< Save as file action.
    QAction *exportAct;             ///< Export file action.
    QAction *closeAct;              ///< Close selected objects.
    QAction *quitAct;               ///< Quit application.
    QAction *axesAct;               ///< Show axes action.
    QAction *gridAct;               ///< Show grid action.
    QAction *solidAct;              ///< Solid mode on.
    QAction *wireframeAct;          ///< Wireframe mode on.
    QAction *solidwithedgesAct;     ///< Solid mode with edges on.
    QAction *infoAct;               ///< Show/hide mesh info.
    QAction *frontAct;              ///< Set front view.
    QAction *backAct;               ///< Set back view.
    QAction *leftAct;               ///< Set left view.
    QAction *rightAct;              ///< Set right view.
    QAction *topAct;                ///< Set top view.
    QAction *bottomAct;             ///< Set bottom view.
    QAction *centerAct;             ///< Reset view to center.
    QAction *selectAllAct;          ///< Select all objects.
    QAction *selectInverseAct;      ///< Select inverse.
    QAction *undoAct;               ///< Undo.
    QAction *redoAct;               ///< Redo.
    QAction *propertiesAct;         ///< Properties dialog.
    QToolButton* openButton;        ///< Open file button.
    QToolButton* saveButton;        ///< Save file button.
    QToolButton* undoButton;        ///< Undo button.
    QToolButton* redoButton;        ///< Redo button.
    QToolButton* closeButton;       ///< Close file button.
};

#endif // WINDOW_H
