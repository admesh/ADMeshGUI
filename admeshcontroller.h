// (c) 2015 David Vyvlečka, AGPLv3

#ifndef ADMESHCONTROLLER_H
#define ADMESHCONTROLLER_H

#include <QGLShaderProgram>
#include <QLabel>
#include <QListView>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QObject>
#include <vector>
#include <QList>
#include <iostream>
#include <cstdio>
#include <ctime>
#include "data.h"
#include "meshobject.h"
#include "historylist.h"

using namespace std;

/*!
 * \brief Controls loaded MeshObjects and drawing
 */
class admeshController : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Default constructor
     */
    explicit admeshController(QObject *parent = 0);

    /*!
     * \brief Destructs holded MeshObjects
     */
    ~admeshController();

    /*!
     * \brief Draws all MeshObjects to the scene
     * \param program Shader program used to draw objects
     */
    void drawAll(QGLShaderProgram *program);

    /*!
     * \brief Sets drawing colors
     *
     * Sets both default color and different color used to draw backfaces
     *
     * \param argc color Default color
     * \param args badColor Backfaces color
     */
    void setDrawColor(QColor color,QColor badColor);

    /*!
     * \brief Set history limit to history list used.
     *
     * \param lim Limit in MB.
     */
    void setHistoryLimit(int lim);

    /*!
     * \brief Get info about all selected meshes.
     * \return string containing formated info.
     */
    QString getInfo();

    /*!
     * \brief Add UI items pointers to be accessible from controller.
     * \param l Status bar label
     * \param v ListView for selection
     */
    void addUIItems(QLabel *l,QListView *v);

    /*!
     * \brief Write settings to config.
     */
    void writeSettings();

signals:
    /*!
     * \brief Sends signal to rendering widget to redraw
     *
     * Sent always after manipulation with STL files
     */
    void reDrawSignal();

    /*!
     * \brief Sends signal to recalculate view according to model size.
     */
    void reCalculatePosition();

    /*!
     * \brief Sends signal to enable or disable edit panel.
     *
     * 0 objects in scene = disabled edit
     */
    void enableEdit(bool);

    /*!
     * \brief Sends signal to set all scale spinboxes to same value.
     */
    void scaleSignal(double);

    /*!
     * \brief Sends signal to enable or disable Undo in menu and in toolbar.
     */
    void allowUndo(bool);

    /*!
     * \brief Sends signal to enable or disable Redo in menu and in toolbar.
     */
    void allowRedo(bool);

    /*!
     * \brief Sends signal to enable or disable Save in menu and in toolbar.
     */
    void allowSave(bool);

    /*!
     * \brief Sends signal to enable or disable Save as in menu.
     */
    void allowSaveAs(bool);

    /*!
     * \brief Sends signal to enable or disable Export in menu.
     */
    void allowExport(bool);

    /*!
     * \brief Sends signal to enable or disable Close in menu and in toolbar.
     */
    void allowClose(bool);

public slots:
    /*!
     * \brief Handle selection in ListView.
     *
     * Toggle active state of item under given index.
     *
     * \param modelindex index of item to change.
     */
    void handleSelectionChanged(QModelIndex modelindex);

    /*!
     * \brief Set rendering mode.
     *
     * Sets solid/wireframe or solid mode with thick edges.
     *
     * \param m Mode selected.
     */
    void setMode(int m);

    /*!
     * \brief Get count of selected objects.
     *
     * \return count
     */
    int selectedCount();

    /*!
     * \brief Draw into picking offsreen buffer.
     *
     * Draws all objects into scene. Each object with own different color used to determine which one was clicked.
     *
     * \param program Shader program used.
     */
    void drawPicking(QGLShaderProgram *program);

    /*!
     * \brief Set object active by given index.
     * \param id Index to be selected.
     */
    void setActiveByIndex(GLuint id);

    /*!
     * \brief Set all objects active.
     */
    void setAllActive();

    /*!
     * \brief Set all objects ainctive.
     */
    void setAllInactive();

    /*!
     * \brief Inverse all objects active state.
     */
    void setAllInverseActive();

    /*!
     * \brief Hide all selected items.
     */
    void hide();

    /*!
     * \brief Unhide all selected items.
     */
    void unhide();

    /*!
     * \brief Unhide all items.
     */
    void unhideAll();

    /*!
     * \brief Open dialog window to open STL file
     */
    void openSTL();

    /*!
     * \brief Open STL file given by parameter
     */
    void openSTLbyName(const char* file);

    /*!
     * \brief Close selected objects
     */
    void closeSTL();

    /*!
     * \brief Save STL file as binary or ascii
     */
    void saveAs();

    /*!
     * \brief Save selected STL files in default format
     */
    void save();

    /*!
     * \brief Save one MeshObject. Native save if filename is valid, otherwise save as.
     *
     * \param object Pointer to MeshObject to save
     */
    void saveObject(MeshObject* object);

    /*!
     * \brief Ask for save of unsaved files upon application close.
     */
    bool saveOnClose();

    /*!
     * \brief Export STL file as OFF, VRML, DXF or OBJ
     */
    void exportSTL();

    /*!
     * \brief Get maximal diameter of last object in list. Called by RenderingWidget once new file is loaded.
     *
     * \return diameter
     */
    float getMaxDiameter();

    /*!
     * \brief Set scale versor X factor.
     * \param factor Factor
     */
    void setVersorX(double factor);

    /*!
     * \brief Set scale versor Y factor.
     * \param factor Factor
     */
    void setVersorY(double factor);

    /*!
     * \brief Set scale versor Z factor.
     * \param factor Factor
     */
    void setVersorZ(double factor);

    /*!
     * \brief Use versor or not.
     */
    void setVersor();

    /*!
     * \brief Scale selected model by scale factor.
     */
    void scale();

    /*!
     * \brief Mirror selected model by XY plane.
     */
    void mirrorXY();

    /*!
     * \brief Mirror selected model by YZ plane.
     */
    void mirrorYZ();

    /*!
     * \brief Mirror selected model by XZ plane.
     */
    void mirrorXZ();

    /*!
     * \brief Set rotation angle.
     * \param angle Angle of rotation.
     */
    void setRot(double angle);

    /*!
     * \brief Rotate selected model by angle along X axis.
     */
    void rotateX();

    /*!
     * \brief Rotate selected model by angle along Y axis.
     */
    void rotateY();

    /*!
     * \brief Rotate selected model by angle along Z axis.
     */
    void rotateZ();

    /*!
     * \brief Set X translation factor.
     * \param factor Factor of translation.
     */
    void setXTranslate(double factor);

    /*!
     * \brief Set Y translation factor.
     * \param factor Factor of translation.
     */
    void setYTranslate(double factor);

    /*!
     * \brief Set Z translation factor.
     * \param factor Factor of translation.
     */
    void setZTranslate(double factor);

    /*!
     * \brief Switch between relative and non-relative translation.
     */
    void setRelativeTranslate();

    /*!
     * \brief Translate selected model with factors given.
     */
    void translate();

    /*!
     * \brief Translate selected model to be centered around origin.
     */
    void center();

    /*!
     * \brief Snap selected model to zero Z coordinate.
     */
    void snapZ();

    /*!
     * \brief Reverse all facets orientation of selected models.
     */
    void reverseAll();

    /*!
     * \brief Set fix all flag.
     */
    void setFixAllFlag();

    /*!
     * \brief Set exact flag.
     */
    void setExactFlag();

    /*!
     * \brief Set tolerance flag.
     */
    void setToleranceFlag();

    /*!
     * \brief Set tolerance.
     * \param val Tolerance value.
     */
    void setTolerance(double val);

    /*!
     * \brief Set increment flag.
     */
    void setIncrementFlag();

    /*!
     * \brief Set increment.
     * \param val Increment value;
     */
    void setIncrement(double val);

    /*!
     * \brief Set nearby flag.
     */
    void setNearbyFlag();

    /*!
     * \brief Set iterations.
     * \param val Number of iterations.
     */
    void setIterations(int val);

    /*!
     * \brief Set remove unconnected vertices flag.
     */
    void setRemoveUnconnectedFlag();

    /*!
     * \brief Set fill holes flag.
     */
    void setFillHolesFlag();

    /*!
     * \brief Set repair normal directions flag.
     */
    void setNormalDirFlag();

    /*!
     * \brief Set repair normal values flag.
     */
    void setNormalValFlag();

    /*!
     * \brief Repair selected model according to selected flags.
     */
    void repair();

    /*!
     * \brief Merge all active models into one.
     */
    void merge();

    /*!
     * \brief Split all active models by shells if possible.
     */
    void split();

    /*!
     * \brief Duplicate all active models.
     */
    void duplicate();

    /*!
     * \brief Undo last operation.
     */
    void undo();

    /*!
     * \brief Redo last undoed operation.
     */
    void redo();

private:
    /*!
     * \brief Add current items state into history list.
     */
    void pushHistory();
    /*!
     * \brief Prepare items for next operation.
     *
     * Active items are deep copied.
     * Reference for each inactive item is raised.
     */
    void renewList();

    /*!
     * \brief Update listView of items according to current item list.
     */
    void renewListView();

    /*!
     * \brief Add name of given item at the end of ListView.
     *
     * \param item Item to be processed.
     */
    void addItemToView(MeshObject* item);

    /*!
     * \brief Allow use of selected actions based on scene state.
     *
     * E.g. allow undo/redo.
     */
    void allowFunctions();

    /*!
     * \brief Allow use of selected actions based on selection.
     *
     * E.g. allow save as/export.
     */
    void allowSelectionFunctions();

    historyList history;    ///< History list
    QList <MeshObject*> objectList;  ///< List of currently drawn objects.
    QVector3D color;        ///< Default color
    QVector3D badColor;     ///< Backface color
    QLabel* statusBar;      ///< Status bar pointer.
    QListView *listView;    ///< ListView pointer.
    QStandardItemModel *listModel;    ///< Model to be displayed in ListView.
    QList <MeshObject*>::size_type count;     ///< Count of all objects.
    QIcon hiddenIcon;         ///< Hidden icon
    QIcon visibleIcon;      ///< Visible icon
    int mode;               ///< Solid or wireframe or both mode
    float versor[3];        ///< Scale versor
    bool useVersor;         ///< Use versor or not
    float rot;              ///< Rotation angle
    float x_translate;      ///< X translation factor
    float y_translate;      ///< Y translation factor
    float z_translate;      ///< Z translation factor
    bool rel_translate;     ///< Relative translation
    bool fixall_flag;
    bool exact_flag;
    bool tolerance_flag;
    float tolerance;
    bool increment_flag;
    float increment;
    bool nearby_flag;
    int iterations;
    bool remove_unconnected_flag;
    bool fill_holes_flag;
    bool normal_directions_flag;
    bool normal_values_flag;
    clock_t start_time;
};

#endif // ADMESHCONTROLLER_H
