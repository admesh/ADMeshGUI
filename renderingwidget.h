#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLWidget>
#include <QGLFunctions>
#include <QMatrix4x4>
#include <QVector2D>
#include <QGLShaderProgram>
#include <QOpenGLFramebufferObject>
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
 * Reimplements QOpenGLWidget (Qt version >= 5.4).
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
     * Draws 3D first: main content + corner axes.
     * Than draws 2D text over 3D content using QPainter.
     * Calculates and sets PVM matrix uniform.
     */
    void paintGL();

    /*!
     * \brief Reimplemented resize method.
     *
     * Calles upon resize of window.
     */
    void resizeGL(int width, int height);

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
     * \brief Set front view angles.
     */
    void setFrontView();

    /*!
     * \brief Set back view angles.
     */
    void setBackView();

    /*!
     * \brief Set left view angles.
     */
    void setLeftView();

    /*!
     * \brief Set right view angles.
     */
    void setRightView();

    /*!
     * \brief Set top view angles.
     */
    void setTopView();

    /*!
     * \brief Set bottom view angles.
     */
    void setBottomView();

    /*!
     * \brief Changes axes mode. ON/OFF
     */
    void toggleAxes();

    /*!
     * \brief Changes grid mode. ON/OFF
     */
    void toggleGrid();

    /*!
     * \brief View or hide mesh info.
     */
    void toggleInfo();

    /*!
     * \brief Updates scene when necessary.
     */
    void reDraw();

    /*!
     * \brief Update scene zoom according to size of open file.
     */
    void reCalculatePosition();

    /*!
     * \brief Reset camera translation to zero.
     */
    void centerPosition();

    /*!
     * \brief Toggle Shift key pressed information.
     */
    void toggleShift();

signals:

private:
    /*!
     * \brief Initialize axes vertex buffer object and fill it.
     */
    void initAxes();

    /*!
     * \brief Initialize grid vertex buffer object and fill it.
     */
    void initGrid();

    /*!
     * \brief Convert point in 3D scene to screen coordinates.
     * \param worldCoords Coordinates of 3D point.
     * \return Coordinates of point in screen space.
     */
    QVector2D getScreenCoords(QVector3D worldCoords);

    /*!
     * \brief Draw actual info.
     * \param painter Painter used for drawing.
     */
    void drawInfo(QPainter *painter);

    /*!
     * \brief Draw labels for corner axes.
     * \param painter Painter used for drawing.
     */
    void drawLabels(QPainter *painter);

    /*!
     * \brief Render x,y and z axes.
     */
    void drawAxes();

    /*!
     * \brief Render small corner x,y and z axes.
     */
    void drawSmallAxes();

    /*!
     * \brief Render x,y grid.
     */
    void drawGrid();

    /*!
     * \brief Normalizes camera rotation angles.
     */
    void normalizeAngles();

    /*!
     * \brief Calculate view matrix based on Euler angles.
     */
    void getCamPos();

    /*!
     * \brief Recalculate grid step according to current zoom factor.
     *
     * Calculated in range of {1,2,3,4,5,10,15,..} and further in multiples of 5.
     */
    void recalculateGridStep();

    /*!
     * \brief Do color based object picking.
     *
     * Uses offscreen color + depth framebuffer for drawing. Based on color of pixel clicked.
     */
    void doPicking();

    QBasicTimer timer;              ///< Timer used to regular redrawing.

    QGLShaderProgram program;       ///< Common shader program.
    QGLShaderProgram pick_program;  ///< Picking shader program.
    QMatrix4x4 projection;          ///< Projection matrix.
    QMatrix4x4 orthographic;        ///< Orthographic projection matrix.
    QMatrix4x4 view;                ///< View matrix.
    QMatrix4x4 smallView;           ///< No zoom view.
    QMatrix4x4 model;               ///< Model matrix.

    QOpenGLFramebufferObjectFormat pickFboFormat;  ///< Framebuffer used offscreen for object picking.

    GLuint axes_vbo;                ///< Vertex buffer object for axes.
    GLuint grid_vbo;                ///< Vertex buffer object for grid.

    GLfloat angleX;                 ///< X axis angle.
    GLfloat angleY;                 ///< Y axis angle.

    GLfloat xPos;                   ///< Camera X position.
    GLfloat yPos;                   ///< Camera Y position.
    GLfloat zPos;                   ///< Camera Z position.

    GLfloat zoom;                   ///< Camera zoom factor.

    QVector4D smallAxesBox;         ///< Dimension and position of small axes box in screen coordinates.

    bool Axes;                      ///< Axes mode on/off.
    bool Grid;                      ///< Grid mode on/off.
    bool Info;                      ///< Info on/off.

    bool selection;                 ///< Object picking action.

    bool shiftPressed;              ///< Shift key pressed.

    QPoint lastPos;                 ///< Last clicked position (LMB).
    QPoint lastTransPos;            ///< Last clicked position (MMB).
    QPoint lastSelectionPos;        ///< Last clicked position (RMB).

    GLfloat xTrans;                 ///< X direction translation factor.
    GLfloat yTrans;                 ///< Y direction translation factor.

    int gridStep;                   ///< Step between grid lines.

    admeshController *controller;   ///< Pointer to main controller.
};

#endif // MYGLWIDGET_H
