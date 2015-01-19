#include <QtWidgets>
#include <QtOpenGL>
#include <QMessageBox>
#include "renderingwidget.h"

RenderingWidget::RenderingWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{    
    Axes = true;
    SolidMode = false;
    xPos = 1.0f;
    yPos = 0.5f;
    zPos = 1.0f;
    angleX = 0.0f;
    angleY = 70.0f;
    zoom = 100.0f;    
}


RenderingWidget::~RenderingWidget()
{
    stl_close(&stlfile);
    glDeleteBuffers(1, &axes_vbo);
}


void RenderingWidget::setController(admeshController* cnt)
{
    controller = cnt;
}

QSize RenderingWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize RenderingWidget::sizeHint() const
{
    return QSize(DEFAULT_RES_X, DEFAULT_RES_Y);
}

void RenderingWidget::setXRotation(int angle)
{
    angleX = angle;
    update();
}

void RenderingWidget::setYRotation(int angle)
{
    angleY = angle;
    update();
}

void RenderingWidget::toggleAxes()
{
    if(Axes)Axes = false;
    else Axes = true;
    update();
}

void RenderingWidget::toggleMode()
{
    makeCurrent();
    if(SolidMode){
        SolidMode = false;
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        controller->setDrawColor(BLACK);
    }else{
        SolidMode = true;
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        controller->setDrawColor(GREEN);
    }
    reDraw();
}

void RenderingWidget::initializeGL()
{
    initializeGLFunctions();
    initShaders();
    glGenBuffers(1, &axes_vbo);
    initAxes();
    glClearColor(1.0,1.0,1.0,1.0);
    glEnable(GL_DEPTH_TEST);
    timer.start(33, this);
    toggleMode();
    reDraw();
}

void RenderingWidget::initShaders(){
    if (!program.addShaderFromSourceFile(QGLShader::Vertex, ":/vshader.glsl")) close();

    if (!program.addShaderFromSourceFile(QGLShader::Fragment, ":/fshader.glsl")) close();

    if (!program.link()) close();

    if (!program.bind()) close();
}

void RenderingWidget::timerEvent(QTimerEvent *)
{
    update();
}

void RenderingWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(Axes){
        drawAxes();
    }
    getCamPos();
    QMatrix4x4 model;
    model.setToIdentity();
    model.rotate(90, -1.0f,0.0f,0.0f); //rotate to OpenGL axes system
    program.setUniformValue("mvp_matrix", projection * view * model);
    controller->drawAll(&program);
}

void RenderingWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    projection.setToIdentity();
    projection.perspective(PERSPECTIVE, (GLfloat)width/(GLfloat)height, MIN_VIEW_DISTANCE, MAX_VIEW_DISTANCE);
}

void RenderingWidget::drawInfo(QPainter *painter) {
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    QString text;
    QTextStream(&text) << _("Camera angleX: ") << angleX << endl <<_("Camera angleY: ") << angleY << endl <<
                          _("Camera distance: ") << zoom << endl <<_("Solid mode: ");
    if(SolidMode) QTextStream(&text) << _("ON") << endl << _("Axes: ");
    else QTextStream(&text) << _("OFF") << endl << _("Axes: ");
    if(Axes) QTextStream(&text) << _("ON") << endl;
    else QTextStream(&text) << _("OFF") << endl;
    QFontMetrics metrics = QFontMetrics(font());
    int border = qMax(6, metrics.leading());
    QRect rect = metrics.boundingRect(0, 0, width()/4 - 2*border, int(height()/2), Qt::AlignLeft | Qt::TextWordWrap, text);
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->setPen(Qt::black);
    painter->fillRect(QRect(0, 0, width()/5, height()/3), QColor(1, 1, 1, 1));
    painter->drawText(rect.width()/10, border, rect.width(), rect.height(), Qt::AlignLeft | Qt::TextWordWrap, text);
    if(!this->SolidMode)glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
 }

void RenderingWidget::getCamPos()
{
    xPos = zoom * sin(angleY*(M_PI/180)) * sin(angleX*(M_PI/180));
    yPos = zoom * cos(angleY*(M_PI/180));
    zPos = zoom * sin(angleY*(M_PI/180)) * cos(angleX*(M_PI/180));

    GLfloat dt=1.0f; //Small difference to get second point

    GLfloat upX=zoom * sin(angleY*(M_PI/180)-dt) * sin(angleX*(M_PI/180)) -xPos;
    GLfloat upY=zoom * cos(angleY*(M_PI/180)-dt) -yPos;
    GLfloat upZ=zoom * sin(angleY*(M_PI/180)-dt) * cos(angleX*(M_PI/180)) -zPos;

    view.setToIdentity();
    view.lookAt (QVector3D(xPos, yPos, zPos), QVector3D(0.0, 0.0, 0.0), QVector3D(upX, upY, upZ));
}

/*void RenderingWidget::paintEvent(QPaintEvent *event)
{
    makeCurrent();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    qglClearColor(Qt::white);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    static GLfloat lightPosition[4] = { 0, 0, 10, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    this->resizeGL(width(),height());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    getCamPos();
    draw();

    glShadeModel(GL_FLAT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE); //To display text rectangle

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    QPainter painter(this);
    drawInfo(&painter);

    event->accept();
}*/

void RenderingWidget::normalizeAngles()
{
    if(angleX > 360.0f) angleX = fmod((double)angleX,360.0);
    if(angleY > 360.0f) angleY = fmod((double)angleY,360.0);
    if(angleX < 0.0f) angleX = 360.0f - angleX;
    if(angleY < 0.0f) angleY = 360.0f - angleY;
}

void RenderingWidget::wheelEvent(QWheelEvent* event)
{
    GLfloat tmp = zoom - event->delta()/ZOOM_SPEED;
    if(tmp > MIN_ZOOM && tmp < MAX_ZOOM) zoom = tmp;
}

void RenderingWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void RenderingWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        int dx = event->x() - lastPos.x();
        int dy = event->y() - lastPos.y();

        angleY += dy;
        if(angleY>180)angleX -= dx; //take care of opposite rotation upside down
        else angleX +=dx;

        normalizeAngles();
        lastPos = event->pos();
    }
}

void RenderingWidget::initAxes(){
    GLfloat vertices[]={
       AXIS_SIZE, 0.0 , 0.0,
       1.0, 1.0, 1.0,
       -AXIS_SIZE, 0.0, 0.0,
       1.0, 1.0, 1.0,
       0.0, AXIS_SIZE, 0.0,
       1.0, 1.0, 1.0,
       0.0, -AXIS_SIZE, 0.0,
       1.0, 1.0, 1.0,
       0.0, 0.0, AXIS_SIZE,
       1.0, 1.0, 1.0,
       0.0, 0.0, -AXIS_SIZE,
       1.0, 1.0, 1.0,
    };
    glBindBuffer(GL_ARRAY_BUFFER, axes_vbo);
    glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
}

void RenderingWidget::drawAxes()
{
    glBindBuffer(GL_ARRAY_BUFFER, axes_vbo);
    int vertexLocation = program.attributeLocation("a_position");
    program.enableAttributeArray(vertexLocation);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, 0);

    int normalLocation = program.attributeLocation("a_normal");
    program.enableAttributeArray(normalLocation);
    glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, (const void *)(sizeof(GLfloat)*3));


    program.setUniformValue("color", RED);
    glDrawArrays(GL_LINES, 0, 2);
    program.setUniformValue("color", GREEN);
    glDrawArrays(GL_LINES, 2, 2);
    program.setUniformValue("color", BLUE);
    glDrawArrays(GL_LINES, 4, 2);
}

void RenderingWidget::reDraw()
{
    update();
}

