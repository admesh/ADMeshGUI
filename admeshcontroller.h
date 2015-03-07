#ifndef ADMESHCONTROLLER_H
#define ADMESHCONTROLLER_H

#include <QGLShaderProgram>
#include <QObject>
#include <vector>
#include "data.h"
#include "meshobject.h"

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
    void setDrawColor(QVector3D color,QVector3D badColor);

    /*!
     * \brief Get info about all selected meshes.
     * \return string containing formated info.
     */
    QString getInfo();

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

public slots:
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
     * \brief Open dialog window to open STL file
     */
    void openSTL();

    /*!
     * \brief Open STL file given by parameter
     */
    void openSTLbyName(const char* file);

    /*!
     * \brief Save STL file as binary or ascii
     */
    void saveAs();

    /*!
     * \brief Export STL file as OFF, VRML, DXF or OBJ
     */
    void exportSTL();

    /*!
     * \brief Get vector [min x, min y, max z] of active file for camera purposes.
     * \return vector of mininaml values.
     */
    QVector3D getMinPosition();

    /*!
     * \brief Set scale factor.
     * \param factor Factor
     */
    void setScale(double factor);

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
     * \brief Set X rotation angle.
     * \param angle Angle of rotation.
     */
    void setXRot(double angle);

    /*!
     * \brief Set Y rotation angle.
     * \param angle Angle of rotation.
     */
    void setYRot(double angle);

    /*!
     * \brief Set Z rotation angle.
     * \param angle Angle of rotation.
     */
    void setZRot(double angle);

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
     * \brief Set reverse a normals flag.
     */
    void setReverseAllFlag();

    /*!
     * \brief Repair selected model according to selected flags.
     */
    void repair();

private:
    vector <MeshObject*> objectList;  ///< List of currently drawn objects.
    MeshObject* stl;        ///< One STL file
    MeshObject* active;     ///< Active STL file
    QVector3D color;        ///< Default color
    QVector3D badColor;     ///< Backface color
    int mode;               ///< Solid or wireframe or both mode
    float m_scale;          ///< Scale factor
    float versor[3];        ///< Scale versor
    bool useVersor;         ///< Use versor or not
    float x_rot;            ///< X rotation angle
    float y_rot;            ///< Y rotation angle
    float z_rot;            ///< Z rotation angle
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
    bool reverse_all_flag;
};

#endif // ADMESHCONTROLLER_H
