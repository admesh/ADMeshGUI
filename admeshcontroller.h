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

private:
    MeshObject* stl;        ///< One STL file
    MeshObject* active;     ///< Active STL file
    QVector3D color;        ///< Default color
    QVector3D badColor;     ///< Backface color
    float m_scale;          ///< Scale factor
};

#endif // ADMESHCONTROLLER_H
