#include <QtWidgets>
#include <QtOpenGL>
#include <QMessageBox>
#include "renderingwidget.h"

RenderingWidget::RenderingWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{    
    Axes = true;
    Grid = false;
    Info = true;
    xPos = 1.0f;
    yPos = 0.5f;
    zPos = 1.0f;
    angleX = 0.0f;
    angleY = 70.0f;
    zoom = 100.0f;
}


RenderingWidget::~RenderingWidget()
{
    glDeleteBuffers(1, &axes_vbo);
    glDeleteBuffers(1, &grid_vbo);
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

void RenderingWidget::toggleGrid()
{
    Grid = !Grid;
    update();
}

void RenderingWidget::toggleAxes()
{
    Axes = !Axes;
    update();
}

void RenderingWidget::toggleInfo()
{
    Info = !Info;
    update();
}

void RenderingWidget::initializeGL()
{
    initializeGLFunctions();
    initShaders();
    glGenBuffers(1, &axes_vbo);
    glGenBuffers(1, &grid_vbo);
    initAxes();
    initGrid();
    glClearColor(1.0,1.0,1.0,1.0);
    glEnable(GL_DEPTH_TEST);
    timer.start(33, this);
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
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.beginNativePainting();

    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    program.bind();
    if(Axes) drawAxes();
    if(Grid) drawGrid();
    getCamPos();
    QMatrix4x4 model;
    model.setToIdentity();
    model.rotate(90, -1.0f,0.0f,0.0f); //rotate to OpenGL axes system
    program.setUniformValue("mvp_matrix", projection * view * model);
    controller->drawAll(&program);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    program.release();

    glDisable(GL_DEPTH_TEST);
    painter.endNativePainting();

    painter.setRenderHint(QPainter::TextAntialiasing);
    if(Info) drawInfo(&painter);
    painter.end();
}

void RenderingWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    projection.setToIdentity();
    projection.perspective(PERSPECTIVE, (GLfloat)width/(GLfloat)height, MIN_VIEW_DISTANCE, MAX_VIEW_DISTANCE);
}

void RenderingWidget::drawInfo(QPainter *painter) {
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    QString text = controller->getInfo();
    QTextStream(&text) << _("Camera angleX: ") << angleX << endl <<_("Camera angleY: ") << angleY << endl;
    QFontMetrics metrics = QFontMetrics(font());
    int border = qMax(6, metrics.leading());
    QRect rect = metrics.boundingRect(0, 0, width()/4 - 2*border, int(height()/2), Qt::AlignLeft | Qt::TextWordWrap, text);
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->setPen(Qt::black);
    painter->fillRect(QRect(0, 0, width()/5, height()/3), QColor(1, 1, 1, 1));
    painter->drawText(rect.width()/10, border, rect.width(), rect.height(), Qt::AlignLeft | Qt::TextWordWrap, text);
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

void RenderingWidget::initGrid(){
    int size= (GRID_SIZE+1)* 4 * 4; // 4*4 for 4 sides of vertexes * 4 float
    GLfloat *vertices= new GLfloat[size];
    for(int i = 0;i<= GRID_SIZE*2; i++){
        vertices[i*4]=-GRID_SIZE;
        vertices[i*4 + 1]=i-GRID_SIZE;
        vertices[i*4 + 2]=GRID_SIZE;
        vertices[i*4 + 3]=i-GRID_SIZE;
    }
    int ind = GRID_SIZE*2*4+4;
    for(int i = 0;i<= GRID_SIZE*2; i++){
        vertices[ind + (i*4)]=i-GRID_SIZE;
        vertices[ind + (i*4+1)]=-GRID_SIZE;
        vertices[ind + (i*4+2)]=i-GRID_SIZE;
        vertices[ind + (i*4+3)]=GRID_SIZE;
    }
    glBindBuffer(GL_ARRAY_BUFFER, grid_vbo);
    glBufferData(GL_ARRAY_BUFFER, (GRID_SIZE+1) * 4 * 4 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
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

void RenderingWidget::drawGrid()
{
    glBindBuffer(GL_ARRAY_BUFFER, grid_vbo);
    int vertexLocation = program.attributeLocation("a_position");
    program.enableAttributeArray(vertexLocation);
    glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2, 0);

    program.setUniformValue("color", BLACK);
    glDrawArrays(GL_LINES, 0, (GRID_SIZE)*8 +4);
}

void RenderingWidget::reDraw()
{
    update();
}

void RenderingWidget::reCalculatePosition()
{
    QVector3D vec = controller->getMinPosition();
    xPos = 1.0f;
    yPos = 0.5f;
    zPos = 1.0f;
    angleX = 0.0f;
    angleY = 70.0f;
    if(!vec.isNull()) zoom = 2.5*sqrt(pow(vec.x(),2)+pow(vec.y(),2)+pow(vec.z(),2));
    else zoom = 100;
}

