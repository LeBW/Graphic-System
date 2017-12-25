#include "glwidget.h"
#include <QOpenGLFunctions>
#include <QTimer>

//构造函数
GLWidget::GLWidget(QWidget *parent): QOpenGLWidget(parent)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);
    setFocusPolicy(Qt::StrongFocus);
   //QMetaObject::invokeMethod(this,"updateGL",Qt::QueuedConnection);
}
ThreeDimensionWidget::ThreeDimensionWidget(QWidget *parent): QOpenGLWidget(parent) {

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(20);
}

void GLWidget::initializeGL() {
   initializeOpenGLFunctions();
   glClearColor(1, 1, 1, 1);
   glMatrixMode(GL_PROJECTION);
   glOrtho(0, 800, 600, 0, 0, 1);
   glColor3f(0, 0, 0);
}


void GLWidget::paintGL() {
    /*glBegin(GL_LINES);
    glVertex2d(0, 0);
    glVertex2d(400, 300);
    glEnd();
    */
    if(currentState != 99) {
        drawLines();
        drawCircles();
        drawOvals();
        drawMypolygons();
        drawCutRect();
        if(selectedShape.isSelected == true)
            drawHighligh();
    }
}

void ThreeDimensionWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
}
void ThreeDimensionWidget::paintGL() {
    glRotatef(xrot, 1, 0, 0);
    glRotatef(yrot, 0, 1, 0);
    glRotatef(zrot, 0, 0, 1);
    drawCube();
    xrot = xrot + 0.001;
    yrot = yrot + 0.001;
    zrot = zrot + 0.001;
}

void ThreeDimensionWidget::drawCube() {
    glBegin(GL_QUADS);
    glColor3f(1,1,0.0);
    glVertex3f(0.5, 0.5,-0.5);
    glColor3f(0.0,1,0.0);
    glVertex3f(-0.5, 0.5,-0.5);
    glColor3f(0.0,1,1);
    glVertex3f(-0.5, 0.5, 0.5);
    glColor3f(1,1,1);
    glVertex3f( 0.5, 0.5, 0.5);

    glColor3f(1,0.0,1);
    glVertex3f( 0.5,-0.5, 0.5);
    glColor3f(0.0,0.0,1);
    glVertex3f(-0.5,-0.5, 0.5);
    glColor3f(0.0,0.0,0.0);
    glVertex3f(-0.5,-0.5,-0.5);
    glColor3f(1,0.0,0.0);
    glVertex3f( 0.5,-0.5,-0.5);

    glColor3f(1,1,1);
    glVertex3f( 0.5, 0.5, 0.5);
    glColor3f(0.0,1,1);
    glVertex3f(-0.5, 0.5, 0.5);
    glColor3f(0.0,0.0,1);
    glVertex3f(-0.5,-0.5, 0.5);
    glColor3f(1,0.0,1);
    glVertex3f( 0.5,-0.5, 0.5);

    glColor3f(1,0.0,0.0);
    glVertex3f( 0.5,-0.5,-0.5);
    glColor3f(0.0,0.0,0.0);
    glVertex3f(-0.5,-0.5,-0.5);
    glColor3f(0.0,1,0.0);
    glVertex3f(-0.5, 0.5,-0.5);
    glColor3f(1,1,0.0);
    glVertex3f( 0.5, 0.5,-0.5);

    glColor3f(0.0,1,1);
    glVertex3f(-0.5, 0.5, 0.5);
    glColor3f(0.0,1,0.0);
    glVertex3f(-0.5, 0.5,-0.5);
    glColor3f(0.0,0.0,0.0);
    glVertex3f(-0.5,-0.5,-0.5);
    glColor3f(0.0,0.0,1);
    glVertex3f(-0.5,-0.5, 0.5);

    glColor3f(1,1,0.0);
    glVertex3f( 0.5, 0.5,-0.5);
    glColor3f(1,1,1);
    glVertex3f( 0.5, 0.5, 0.5);
    glColor3f(1,0.0,1);
    glVertex3f( 0.5,-0.5, 0.5);
    glColor3f(1,0.0,0.0);
    glVertex3f( 0.5,-0.5,-0.5);
    glEnd();
}
