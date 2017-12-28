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
    AngleX = 45.0f;
    AngleY = 315.0f;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DITHER);
    glShadeModel(GL_SMOOTH);

    int w = WIDTH, h = HEIGHT;
    GLfloat aspect = (GLfloat)w / (GLfloat)h;
    GLfloat nRange = 200.0f;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION); //将当前矩阵指定为投影模式
    glLoadIdentity();

    //设置三维投影区

    if (w <= h)
    {
        glOrtho(-nRange, nRange, -nRange * aspect, nRange * aspect, -nRange, nRange);
    }
    else
    {
        glOrtho(-nRange, nRange, -nRange / aspect, nRange / aspect, -nRange, nRange);
    }
}
void ThreeDimensionWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    {
        glRotatef(AngleX, 1.0f, 0.0f, 0.0f);
        glRotatef(AngleY, 0.0f, 1.0f, 0.0f);

        glBegin(GL_POLYGON); //前表面
        glColor3ub((GLubyte)255, (GLubyte)255, (GLubyte)255);//颜色设置为白色
        glVertex3f(50.0f, 50.0f, 50.0f);

        glColor3ub((GLubyte)255, (GLubyte)255, (GLubyte)0);//颜色设置为黄色
        glVertex3f(50.0f, -50.0f, 50.0f);

        glColor3ub((GLubyte)255, (GLubyte)0, (GLubyte)0);//颜色设置为红色
        glVertex3f(-50.0f, -50.0f, 50.0f);

        glColor3ub((GLubyte)255, (GLubyte)0, (GLubyte)255);//颜色设置为白色
        glVertex3f(-50.0f, 50.0f, 50.0f);
        glEnd();

        glBegin(GL_POLYGON); //后表面
        glColor3f(0.0f, 1.0f, 1.0f);//颜色设置为青色
        glVertex3f(50.0f, 50.0f, -50.0f);

        glColor3f(0.0f, 1.0f, 0.0f);//颜色设置为绿色
        glVertex3f(50.0f, -50.0f, -50.0f);

        glColor3f(0.0f, 0.0f, 0.0f);//颜色设置为黑色
        glVertex3f(-50.0f, -50.0f, -50.0f);

        glColor3f(0.0f, 0.0f, 1.0f);//颜色设置为蓝色
        glVertex3f(-50.0f, 50.0f, -50.0f);
        glEnd();

        glBegin(GL_POLYGON); //上表面
        glColor3d(0.0, 1.0, 1.0);//颜色设置为青色
        glVertex3f(50.0f, 50.0f, -50.0f);

        glColor3d(1.0, 1.0, 1.0);//颜色设置为白色
        glVertex3f(50.0f, 50.0f, 50.0f);

        glColor3d(1.0, 0.0, 1.0);//颜色设置为品红色
        glVertex3f(-50.0f, 50.0f, 50.0f);

        glColor3d(0.0, 0.0, 1.0);//颜色设置为蓝色
        glVertex3f(-50.0f, 50.0f, -50.0f);
        glEnd();

        glBegin(GL_POLYGON); //下表面
        glColor3ub(0u, 255u, 0u);//颜色设置为绿色
        glVertex3f(50.0f, -50.0f, -50.0f);

        glColor3ub(255u, 255u, 0u);//颜色设置为黄色
        glVertex3f(50.0f, -50.0f, 50.0f);

        glColor3ub(255u, 0u, 0u);//颜色设置为红色
        glVertex3f(-50.0f, -50.0f, 50.0f);

        glColor3ub(0u, 0u, 0u);//颜色设置为黑色
        glVertex3f(-50.0f, -50.0f, -50.0f);
        glEnd();

        glBegin(GL_POLYGON); //左表面
        glColor3ub((GLubyte)255, (GLubyte)255, (GLubyte)255);//颜色设置为白色
        glVertex3f(50.0f, 50.0f, 50.0f);

        glColor3ub((GLubyte)0, (GLubyte)255, (GLubyte)255);//颜色设置为青色
        glVertex3f(50.0f, 50.0f, -50.0f);

        glColor3ub((GLubyte)0, (GLubyte)255, (GLubyte)0);//颜色设置为绿色
        glVertex3f(50.0f, -50.0f, -50.0f);

        glColor3ub((GLubyte)255, (GLubyte)255, (GLubyte)0);//颜色设置为黄色
        glVertex3f(50.0f, -50.0f, 50.0f);
        glEnd();

        glBegin(GL_POLYGON); //右表面
        glColor3f(1.0f, 0.0f, 1.0f);//颜色设置为品红色
        glVertex3f(-50.0f, 50.0f, 50.0f);

        glColor3f(0.0f, 0.0f, 1.0f);//颜色设置为蓝色
        glVertex3f(-50.0f, 50.0f, -50.0f);

        glColor3f(0.0f, 0.0f, 0.0f);//颜色设置为黑色
        glVertex3f(-50.0f, -50.0f, -50.0f);

        glColor3f(1.0f, 0.0f, 0.0f);//颜色设置为红色
        glVertex3f(-50.0f, -50.0f, 50.0f);
        glEnd();
    }
    glPopMatrix();
    AngleX += 5;
    AngleY -= 5;
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
