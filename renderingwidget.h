#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>
#include <QOpenGLWidget>
#include <QGLFunctions>
#include <QMatrix4x4>
#include <QVector2D>
#include <QGLShaderProgram>
#include <QBasicTimer>
#include <QBrush>
#include <QFont>
#include <QImage>
#include <QPen>
#include <GL/glu.h>
#include "data.h"
#include "admeshcontroller.h"

/*!
 * \brief Main rendering class
 *
 * Reimplements QOpenGLWidget (Qt version > 5.4).
 * Handles drawing and direct user input.
 */
class RenderingWidget : public QOpenGLWidget, protected QGLFunctions
{
    Q_OBJECT
public:
    /*!
     * \brief Constructor. Sets default values.
     */
    explicit  RenderingWidget(QWidget *parent = 0);

    /*!
     * \brief Sets controller used.
     *
     * Necessary for calling drawAll method.
     *
     * \param cnt Controller to be set.
     */
    void setController(admeshController* cnt);

    /*!
     * \brief Destructor.
     */
    ~ RenderingWidget();

protected:
    /*!
     * \brief Initializes OpenGL. Makes all default calls.
     */
    void initializeGL();

    /*!
     * \brief Initializes Shaders from source files.
     */
    void initShaders();

    /*!
     * \brief Reimplemented main drawing method.
     *
     * Makes all drawing calls each time widget is updated.
     * Calculates and sets PVM matrix uniform.
     */
    void paintGL();

    /*!
     * \brief Reimplemented resize method.
     *
     * Calles upon resize of window.
     */
    void resizeGL(int width, int height);
    //void paintEvent(QPaintEvent *event);

    /*!
     * \brief Reimplemented timer. Send update(); signal.
     * \param e Timer event sent.
     */
    void timerEvent(QTimerEvent *e);

    /*!
     * \brief Reimplemented method. Gives minimal size of widget.
     */
    QSize minimumSizeHint() const;

    /*!
     * \brief Reimplemented method. Gives default size of widget.
     */
    QSize sizeHint() const;

    /*!
     * \brief Reimplemented method. Handles mouse wheel to zoom.
     * \param event Wheel event sent.
     */
    void wheelEvent(QWheelEvent* event);

    /*!
     * \brief Reimplemented method. Handles mouse button pressed.
     * \param event Press event sent.
     */
    void mousePressEvent(QMouseEvent *event);

    /*!
     * \brief Reimplemented method. Handles mouse move.
     * \param event Mouse move event sent.
     */
    void mouseMoveEvent(QMouseEvent *event);

public slots:    
    /*!
     * \brief Set X angle rotation.
     * \param angle Angle value.
     */
    void setXRotation(int angle);

    /*!
     * \brief Set Y angle rotation.
     * \param angle Angle value.
     */
    void setYRotation(int angle);

    /*!
     * \brief Changes axes mode. ON/OFF
     */
    void toggleAxes();

    /*!
     * \brief Changes between solid and wireframe rendering.
     */
    void toggleMode();

    /*!
     * \brief Updates scene when necessary.
     */
    void reDraw();

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);

private:
    /*!
     * \brief Initialize axes vertex buffer object and fill it.
     */
    void initAxes();

    /*!
     * \brief Draw actual info.
     * \param painter Painter used for drawing.
     */
    void drawInfo(QPainter *painter);

    /*!
     * \brief Render x,y and z axes.
     */
    void drawAxes();

    /*!
     * \brief Normalizes camera rotation angles.
     */
    void normalizeAngles();

    /*!
     * \brief Calculate view matrix based on Euler angles.
     */
    void getCamPos();

    QBasicTimer timer;              ///< Timer used to regular redrawing.

    QGLShaderProgram program;       ///< Common shader program.
    QMatrix4x4 projection;          ///< Projection matrix.
    QMatrix4x4 view;                ///< View matrix.

    GLuint axes_vbo;                ///< Vertex buffer object for axes.

    GLfloat angleX;                 ///< X axis angle.
    GLfloat angleY;                 ///< Y axis angle.

    GLfloat xPos;                   ///< Camera X position.
    GLfloat yPos;                   ///< Camera Y position.
    GLfloat zPos;                   ///< Camera Z position.

    GLfloat zoom;                   ///< Camera zoom factor.

    bool Axes;                      ///< Axes mode on/off.
    bool SolidMode;                 ///< Solid mode on/off.

    QPoint lastPos;                 ///< Last clicked position.

    stl_file stlfile;
    admeshController *controller;   ///< Pointer to main controller.
};

#endif // MYGLWIDGET_H
