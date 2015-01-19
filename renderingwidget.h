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


class RenderingWidget : public QOpenGLWidget, protected QGLFunctions
{
    Q_OBJECT
public:
    explicit  RenderingWidget(QWidget *parent = 0);
    void setController(admeshController* cnt);
    ~ RenderingWidget();

protected:
    void initializeGL();
    void initShaders();
    void paintGL();
    void resizeGL(int width, int height);
    //void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *e);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void wheelEvent(QWheelEvent* event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public slots:    
    void setXRotation(int angle);
    void setYRotation(int angle);
    void toggleAxes();
    void toggleMode();
    void reDraw();

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);

private:
    void initAxes();
    void drawInfo(QPainter *painter);
    void drawAxes();
    void draw();
    void normalizeAngles();
    void getCamPos();

    QBasicTimer timer;

    QGLShaderProgram program;
    QMatrix4x4 projection;
    QMatrix4x4 view;

    GLuint axes_vbo;

    GLfloat angleX;
    GLfloat angleY;

    GLfloat xPos;
    GLfloat yPos;
    GLfloat zPos;

    GLfloat zoom;

    bool Axes;
    bool SolidMode;

    QPoint lastPos;

    stl_file stlfile;
    admeshController *controller;
};

#endif // MYGLWIDGET_H
