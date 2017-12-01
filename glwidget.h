#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QWheelEvent>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <iterator>


#define WIDTH 800
#define HEIGHT 600

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    explicit GLWidget(QWidget *parent = 0);

    void initializeGL();
    void paintGL();
//    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
};

//定义像素点
class PixelPoint{
public:
    double x;
    double y;
    PixelPoint(){}
    PixelPoint(double x1, double y1){ x = x1; y = y1; }
};
//定义直线结构
struct Line{
    PixelPoint from;
    PixelPoint to;
    PixelPoint center;
};
//定义圆结构.
//模仿PowerPoint内画圆方法，利用 两个点坐标 来确定圆心和半径并画圆
struct Circle{
    PixelPoint from;
    PixelPoint to;
    PixelPoint center;
    double radius;
};
//定义椭圆的结构。
struct Oval {
    PixelPoint from;
    PixelPoint to;
    PixelPoint center;
};
//定义多边形结构
struct Mypolygon {
    std::vector<PixelPoint> points;
    bool whetherFull = false;   //是否填充
    PixelPoint center;
};
//定义绘画状态
enum CurrentState {
    line, circle, oval, polygon, filledPolygon, edit, parallel, myrotate, zoom
};

//装图形的容器声明
extern std::vector <Line> lines;
extern std::vector <Circle> circles;
extern std::vector <Oval> ovals;
extern std::vector <Mypolygon> mypolygons;
extern CurrentState currentState;

struct SelectedShape {
    bool isSelected = false;
    CurrentState shape;
    int index;
};


extern bool polygonIsDrawing;
extern SelectedShape selectedShape;



void drawLines();
void drawCircles();
void drawOvals();
void drawMypolygons();
void drawHighligh();
#endif // GLWIDGET_H
