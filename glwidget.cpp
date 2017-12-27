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
        drawBezierCurves();
        drawBSplineCurves();
        if(selectedShape.isSelected == true)
            drawHighligh();
        if(curveIsDrawing) {
            drawAuxiliaryLine();
        }
    }
}
void GLWidget::saveImage() {
    unsigned char* image = (unsigned char*)malloc(sizeof(unsigned char)*3*WIDTH*HEIGHT);
    glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, image);
    char buffer[33];
    sprintf(buffer, "capture%d.ppm", nbm);
    nbm++;

    ppmWriter(image, buffer, WIDTH, HEIGHT);
}
void GLWidget::ppmWriter(unsigned char *in, char *name, int dimx, int dimy)
{
    FILE *fp = fopen(name, "wb");
    (void) fprintf(fp, "P6\n%d %d\n255\n", dimx, dimy);
    for(int i = 0; i < dimy; i++) {
        for(int j = 0; j < dimx; j++) {
            static unsigned char color[3];
            int cur = 3*((HEIGHT-i-1)*WIDTH + j);
            color[0] = in[cur];
            color[1] = in[cur+1];
            color[2] = in[cur+2];
            (void) fwrite(color, 1, 3, fp);
        }
    }
    fclose(fp);
}

void ThreeDimensionWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    glOrtho(-4, 4, -3, 3 , -3, 3);
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
    glColor3f(1.0,1.0,0.0);
    glVertex3f( 1.0, 1.0,-1.0);
    glColor3f(0.0,1.0,0.0);
    glVertex3f(-1.0, 1.0,-1.0);
    glColor3f(0.0,1.0,1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glColor3f(1.0,1.0,1.0);
    glVertex3f( 1.0, 1.0, 1.0);

    glColor3f(1.0,0.0,1.0);
    glVertex3f( 1.0,-1.0, 1.0);
    glColor3f(0.0,0.0,1.0);
    glVertex3f(-1.0,-1.0, 1.0);
    glColor3f(0.0,0.0,0.0);
    glVertex3f(-1.0,-1.0,-1.0);
    glColor3f(1.0,0.0,0.0);
    glVertex3f( 1.0,-1.0,-1.0);

    glColor3f(1.0,1.0,1.0);
    glVertex3f( 1.0, 1.0, 1.0);
    glColor3f(0.0,1.0,1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glColor3f(0.0,0.0,1.0);
    glVertex3f(-1.0,-1.0, 1.0);
    glColor3f(1.0,0.0,1.0);
    glVertex3f( 1.0,-1.0, 1.0);

    glColor3f(1.0,0.0,0.0);
    glVertex3f( 1.0,-1.0,-1.0);
    glColor3f(0.0,0.0,0.0);
    glVertex3f(-1.0,-1.0,-1.0);
    glColor3f(0.0,1.0,0.0);
    glVertex3f(-1.0, 1.0,-1.0);
    glColor3f(1.0,1.0,0.0);
    glVertex3f( 1.0, 1.0,-1.0);

    glColor3f(0.0,1.0,1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glColor3f(0.0,1.0,0.0);
    glVertex3f(-1.0, 1.0,-1.0);
    glColor3f(0.0,0.0,0.0);
    glVertex3f(-1.0,-1.0,-1.0);
    glColor3f(0.0,0.0,1.0);
    glVertex3f(-1.0,-1.0, 1.0);

    glColor3f(1.0,1.0,0.0);
    glVertex3f( 1.0, 1.0,-1.0);
    glColor3f(1.0,1.0,1.0);
    glVertex3f( 1.0, 1.0, 1.0);
    glColor3f(1.0,0.0,1.0);
    glVertex3f( 1.0,-1.0, 1.0);
    glColor3f(1.0,0.0,0.0);
    glVertex3f( 1.0,-1.0,-1.0);
    glEnd();
}
