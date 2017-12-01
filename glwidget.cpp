#include "glwidget.h"
#include <QOpenGLFunctions>
#include <QTimer>


GLWidget::GLWidget(QWidget *parent): QOpenGLWidget(parent)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);

   //QMetaObject::invokeMethod(this,"updateGL",Qt::QueuedConnection);
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
    drawLines();
    drawCircles();
    drawOvals();
    drawMypolygons();
    if(selectedShape.isSelected == true)
        drawHighligh();
}

