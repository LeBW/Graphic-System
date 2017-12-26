#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <glwidget.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    GLWidget *glWidget = new GLWidget();
   // ThreeDimensionWidget *threeDimensionWidget = new ThreeDimensionWidget();
    ~MainWindow();

private slots:
    void on_actionline_triggered();

    void on_actioncircle_triggered();

    void on_actionoval_triggered();

    void on_actionpolygon_triggered();

    void on_actionfilledPolygon_triggered();

    void on_actionEdit_triggered();

    void on_actionparallel_triggered();

    void on_actionrotate_triggered();

    void on_actionzoom_triggered();

    void on_actionmakeFull_triggered();

    void on_actioncut_triggered();

    void on_action3D_triggered();

    void on_actionsave_triggered();

    void on_actionbezierCurve_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
