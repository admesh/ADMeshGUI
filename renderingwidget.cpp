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
    model.setToIdentity();
    model.rotate(90, -1.0f,0.0f,0.0f);  //Rotate to OpenGL axes system
    smallAxesBox = QVector4D(5, 5, 105, 105);
    gridStep = 1;
    shiftPressed = false;
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

void RenderingWidget::setFrontView()
{
    angleX = 0;
    angleY = 90;
}

void RenderingWidget::setBackView()
{
    angleX = 180;
    angleY = 90;
}

void RenderingWidget::setLeftView()
{
    angleX = 270;
    angleY = 90;
}

void RenderingWidget::setRightView()
{
    angleX = 90;
    angleY = 90;
}

void RenderingWidget::setTopView()
{
    angleX = 0;
    angleY = 0;
}

void RenderingWidget::setBottomView()
{
    angleX = 0;
    angleY = 180;
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
    selection = false;
    initAxes();
    initGrid();
    glClearColor(1.0,1.0,1.0,1.0);
    glEnable(GL_DEPTH_TEST);
    pickFboFormat.setAttachment(QOpenGLFramebufferObject::Depth);
    pickFboFormat.setTextureTarget(GL_TEXTURE_2D);
    pickFboFormat.setInternalTextureFormat(GL_RGBA8);
    timer.start(33, this);
    recalculateGridStep();
    reDraw();
}

void RenderingWidget::initShaders(){
    if (!program.addShaderFromSourceFile(QGLShader::Vertex, ":/vshader.glsl")) close();

    if (!program.addShaderFromSourceFile(QGLShader::Fragment, ":/fshader.glsl")) close();

    if (!program.link()) close();

    if (!program.bind()) close();

    if (!pick_program.addShaderFromSourceFile(QGLShader::Vertex, ":/picking_vshader.glsl")) close();

    if (!pick_program.addShaderFromSourceFile(QGLShader::Fragment, ":/picking_fshader.glsl")) close();

    if (!pick_program.link()) close();

    if (!pick_program.bind()) close();

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
    painter.beginNativePainting();      //Start rendering 3D content

    if(selection){
        doPicking();
        selection = false;
    }

    glClearColor(1.0,1.0,1.0,1.0);      //Set OpenGl states
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program.bind();                     //Use shader program
    glViewport(0, 0, width(), height());
    getCamPos();

    program.setUniformValue("mvp_matrix", projection * view * model);   //Draw main window contents
    if(Axes) drawAxes();
    if(Grid) drawGrid();
    controller->drawAll(&program);

    glViewport(smallAxesBox.x(), smallAxesBox.y(), smallAxesBox.z(), smallAxesBox.w()); // xStart, yStart, xWidth, yWidth
    program.setUniformValue("mvp_matrix", orthographic * smallView * model); //Draw corner orthographic axes
    drawSmallAxes();
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDisable(GL_DEPTH_TEST);
    program.release();
    painter.endNativePainting();        //Start rendering 2D content

    painter.setRenderHint(QPainter::TextAntialiasing);
    if(Info) drawInfo(&painter);
    drawLabels(&painter);
    painter.end();
}

void RenderingWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    projection.setToIdentity();
    projection.perspective(PERSPECTIVE, (GLfloat)width/(GLfloat)height, MIN_VIEW_DISTANCE, MAX_VIEW_DISTANCE);
    orthographic.setToIdentity();
    orthographic.ortho (-1.0f,1.0f,-1.0f,1.0f, -100, 100 );
}

void RenderingWidget::doPicking(){
    glViewport(0, 0, width(), height());
    QOpenGLFramebufferObject fbo(width(),height(), pickFboFormat);    
    fbo.bind();
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    pick_program.bind();
    pick_program.setUniformValue("mvp_matrix", projection * view * model);
    controller->drawPicking(&pick_program);
    QImage img = fbo.toImage();
    QRgb color = img.pixel(lastTransPos.x(),lastTransPos.y());
    int id = qRed(color);
    if(shiftPressed){
        controller->setActiveByIndex(id);
    }else{
        controller->setAllInactive();
        controller->setActiveByIndex(id);
    }
    pick_program.release();
    fbo.release();
}

void RenderingWidget::drawInfo(QPainter *painter)
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    QString text = controller->getInfo();
    QTextStream(&text) << _("Camera angleX: ") << angleX << endl <<_("Camera angleY: ") << angleY << endl;
    if(Grid) QTextStream(&text) <<_("Grid step: ") << gridStep << endl;
    QFontMetrics metrics = QFontMetrics(font());
    int border = qMax(6, metrics.leading());
    QRect rect = metrics.boundingRect(0, 0, width()/4 - 2*border, int(height()/2), Qt::AlignLeft | Qt::TextWordWrap, text);
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->setPen(Qt::black);
    painter->fillRect(QRect(0, 0, qMin(width()/2,250), qMin(height(),370)), QColor(1, 1, 1, 1));
    painter->drawText(rect.width()/10, border, rect.width(), rect.height(), Qt::AlignLeft | Qt::TextWordWrap, text);
}

QVector2D RenderingWidget::getScreenCoords(QVector3D worldCoords){
    QVector4D homogCoords = orthographic * smallView * model * QVector4D(worldCoords, 1.0);
    GLfloat X = homogCoords.x() / homogCoords.w();
    GLfloat Y = homogCoords.y() / homogCoords.w();
    return QVector2D(smallAxesBox.x() + smallAxesBox.z() * (X+1)/2,smallAxesBox.y() + smallAxesBox.w() * (Y+1)/2);
}

void RenderingWidget::drawLabels(QPainter *painter)
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    QVector2D screenCoords = getScreenCoords(QVector3D(0.7, 0.5 , -0.55));  // X axis
    painter->setPen(Qt::red);
    painter->drawText(screenCoords.x(),height()-screenCoords.y(),"x");
    screenCoords = getScreenCoords(QVector3D(-0.5, -0.7, -0.55));           // Y axis
    painter->setPen(Qt::green);
    painter->drawText(screenCoords.x(),height()-screenCoords.y(),"y");
    screenCoords = getScreenCoords(QVector3D(-0.5, 0.5, 0.7));              // Z axis
    painter->setPen(Qt::blue);
    painter->drawText(screenCoords.x(),height()-screenCoords.y(),"z");
}

void RenderingWidget::getCamPos()
{
    xPos = sin(angleY*(M_PI/180)) * sin(angleX*(M_PI/180));
    yPos = cos(angleY*(M_PI/180));
    zPos = sin(angleY*(M_PI/180)) * cos(angleX*(M_PI/180));

    GLfloat dt=1.0f; //Small difference to get second point

    GLfloat upX=sin(angleY*(M_PI/180)-dt) * sin(angleX*(M_PI/180)) -xPos;
    GLfloat upY=cos(angleY*(M_PI/180)-dt) -yPos;
    GLfloat upZ=sin(angleY*(M_PI/180)-dt) * cos(angleX*(M_PI/180)) -zPos;

    view.setToIdentity();
    view.translate(xTrans, yTrans, -zoom);
    view.lookAt (QVector3D(xPos, yPos,zPos), QVector3D(0.0, 0.0, 0.0), QVector3D(upX, upY, upZ));

    smallView.setToIdentity();
    smallView.lookAt (QVector3D(xPos, yPos, zPos), QVector3D(0.0, 0.0, 0.0), QVector3D(upX, upY, upZ));
}

void RenderingWidget::normalizeAngles()
{
    if(angleX > 360.0f) angleX = fmod((double)angleX,360.0);
    if(angleY > 360.0f) angleY = fmod((double)angleY,360.0);
    if(angleX < 0.0f) angleX = 360.0f - angleX;
    if(angleY < 0.0f) angleY = 360.0f - angleY;
}

void RenderingWidget::recalculateGridStep()
{
    int factor = (int)(zoom/GRID_SIZE);
    if(factor > 5){
        int remainder = factor % 5;
        factor -= remainder;
    }else{
        factor = qMax(1,factor);
    }
    if(factor != gridStep) {
        gridStep = factor;
        initGrid();
    }
}

void RenderingWidget::toggleShift()
{
    shiftPressed = !shiftPressed;
}

void RenderingWidget::wheelEvent(QWheelEvent* event)
{
    float tmp = zoom;
    if(event->delta()<0){
        tmp *= 1.25;
    }else{
        tmp *= 0.8;
    }
    if(tmp > MIN_ZOOM && tmp < MAX_ZOOM) zoom = tmp;
    recalculateGridStep();
}

void RenderingWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton) lastPos = event->pos();
    if(event->buttons() & Qt::RightButton) {
        lastTransPos = event->pos();
        selection = true;
    }
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
    if(event->buttons() & Qt::RightButton)
    {
        int dx = (event->x() - lastTransPos.x());
        int dy = (event->y() - lastTransPos.y());

        xTrans -= (GLfloat)dx/3;
        yTrans += (GLfloat)dy/3;
        lastTransPos = event->pos();
    }
}

void RenderingWidget::initAxes(){
    GLfloat vertices[]={
       AXIS_SIZE, 0.0 , 0.0,    //Main axes
       1.0, 1.0, 1.0,
       0.0, 0.0, 0.0,
       1.0, 1.0, 1.0,
        0.0, 0.0 , 0.0,
        1.0, 1.0, 1.0,
        -AXIS_SIZE, 0.0, 0.0,
        1.0, 1.0, 1.0,
       0.0, AXIS_SIZE, 0.0,
       1.0, 1.0, 1.0,
       0.0, 0.0, 0.0,
       1.0, 1.0, 1.0,
        0.0, 0.0, 0.0,
        1.0, 1.0, 1.0,
        0.0, -AXIS_SIZE, 0.0,
        1.0, 1.0, 1.0,
       0.0, 0.0, AXIS_SIZE,
       1.0, 1.0, 1.0,
       0.0, 0.0, 0.0,
       1.0, 1.0, 1.0,
        0.0, 0.0, 0.0,
        1.0, 1.0, 1.0,
        0.0, 0.0, -AXIS_SIZE,
        1.0, 1.0, 1.0,           //Small corner axes
       0.5, 0.5 , -0.5, //x
       1.0, 1.0, 1.0,
       -0.5, 0.5, -0.5,
       1.0, 1.0, 1.0,
       -0.5, -0.5, -0.5, //y
       1.0, 1.0, 1.0,
       -0.5, 0.5, -0.5,
       1.0, 1.0, 1.0,
       -0.5, 0.5, 0.5, //z
       1.0, 1.0, 1.0,
       -0.5, 0.5, -0.5,
       1.0, 1.0, 1.0,
    };
    glBindBuffer(GL_ARRAY_BUFFER, axes_vbo);
    glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
}

void RenderingWidget::initGrid(){
    int size= (GRID_SIZE+1)* 4 * 4; // 4*4 for 4 sides of vertexes * 4 float
    GLfloat *vertices= new GLfloat[size];
    for(int i = 0;i<= GRID_SIZE*2; i++){
        vertices[i*4]=-GRID_SIZE*gridStep;
        vertices[i*4 + 1]=(i-GRID_SIZE)*gridStep;
        vertices[i*4 + 2]=GRID_SIZE*gridStep;
        vertices[i*4 + 3]=(i-GRID_SIZE)*gridStep;
    }
    int ind = GRID_SIZE*2*4+4;
    for(int i = 0;i<= GRID_SIZE*2; i++){
        vertices[ind + (i*4)]=(i-GRID_SIZE)*gridStep;
        vertices[ind + (i*4+1)]=-GRID_SIZE*gridStep;
        vertices[ind + (i*4+2)]=(i-GRID_SIZE)*gridStep;
        vertices[ind + (i*4+3)]=GRID_SIZE*gridStep;
    }
    glBindBuffer(GL_ARRAY_BUFFER, grid_vbo);
    glBufferData(GL_ARRAY_BUFFER, (GRID_SIZE+1) * 4 * 4 * sizeof(GLfloat), vertices, GL_DYNAMIC_DRAW);
    delete []vertices;
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
    glDrawArrays(GL_LINES, 0, 4);
    program.setUniformValue("color", GREEN);
    glDrawArrays(GL_LINES, 4, 4);
    program.setUniformValue("color", BLUE);
    glDrawArrays(GL_LINES, 8, 4);
}

void RenderingWidget::drawSmallAxes()
{
    glBindBuffer(GL_ARRAY_BUFFER, axes_vbo);
    int vertexLocation = program.attributeLocation("a_position");
    program.enableAttributeArray(vertexLocation);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, 0);

    int normalLocation = program.attributeLocation("a_normal");
    program.enableAttributeArray(normalLocation);
    glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, (const void *)(sizeof(GLfloat)*3));

    program.setUniformValue("color", RED);
    glDrawArrays(GL_LINES, 12, 2);
    program.setUniformValue("color", GREEN);
    glDrawArrays(GL_LINES, 14, 2);
    program.setUniformValue("color", BLUE);
    glDrawArrays(GL_LINES, 16, 2);
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
    if(!vec.isNull()) zoom = qMin(float(2.5*sqrt(pow(vec.x(),2)+pow(vec.y(),2)+pow(vec.z(),2))),MAX_ZOOM);
    else zoom = 100;
    recalculateGridStep();
}

void RenderingWidget::centerPosition()
{
    xTrans = yTrans = 0;
}

