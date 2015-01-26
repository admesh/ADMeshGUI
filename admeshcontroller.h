#ifndef ADMESHCONTROLLER_H
#define ADMESHCONTROLLER_H

#include <QGLShaderProgram>
#include <QObject>
#include "data.h"
#include "meshobject.h"

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

signals:
    /*!
     * \brief Sends signal to rendering widget to redraw
     *
     * Sent always after manipulation with STL files
     */
    void reDrawSignal();

public slots:
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
     * \brief Set scale factor.
     * \param factor Factor
     */
    void setScale(double factor);

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
    MeshObject* stl;        ///< One STL file
    MeshObject* active;     ///< Active STL file
    QVector3D color;        ///< Default color
    QVector3D badColor;     ///< Backface color
    float m_scale;          ///< Scale factor
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
