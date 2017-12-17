#include "mainwindow.h"
#include <glwidget.h>
#include <QApplication>
#include <vector>
using namespace std;

//定义各图形容器
vector<Line> lines;
vector<Circle> circles;
vector<Oval> ovals;
vector<Mypolygon> mypolygons;
CurrentState currentState = line; //初始状态画直线
Rect* cutRect = NULL;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
