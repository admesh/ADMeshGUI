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

private:
    MeshObject* stl;        ///< One STL file
    MeshObject* active;     ///< Active STL file
    QVector3D color;        ///< Default color
    QVector3D badColor;     ///< Backface color
    float m_scale;          ///< Scale factor
    float x_rot;            ///< X rotation angle
    float y_rot;            ///< Y rotation angle
    float z_rot;            ///< Z rotation angle
};

#endif // ADMESHCONTROLLER_H
