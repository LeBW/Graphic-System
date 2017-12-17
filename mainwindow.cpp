#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusBar->showMessage("Made by LBW");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionline_triggered()
{
    currentState = line;
    selectedShape.isSelected = false;
    if(cutRect != NULL) {
        delete cutRect;
        cutRect = NULL;
    }
}

void MainWindow::on_actioncircle_triggered()
{
    currentState = circle;
    selectedShape.isSelected = false;
    if(cutRect != NULL) {
        delete cutRect;
        cutRect = NULL;
    }
}

void MainWindow::on_actionoval_triggered()
{
    currentState = oval;
    selectedShape.isSelected = false;
    if(cutRect != NULL) {
        delete cutRect;
        cutRect = NULL;
    }
}

void MainWindow::on_actionpolygon_triggered()
{
    currentState = polygon;
    selectedShape.isSelected = false;
    polygonIsDrawing = false;
    if(cutRect != NULL) {
        delete cutRect;
        cutRect = NULL;
    }
}

void MainWindow::on_actionfilledPolygon_triggered()
{
    currentState = filledPolygon;
    selectedShape.isSelected = false;
    polygonIsDrawing = false;
    if(cutRect != NULL) {
        delete cutRect;
        cutRect = NULL;
    }
}

void MainWindow::on_actionEdit_triggered()
{
    currentState = edit;
    if(cutRect != NULL) {
        delete cutRect;
        cutRect = NULL;
    }
}

void MainWindow::on_actionparallel_triggered()
{
    currentState = parallel;
    if(cutRect != NULL) {
        delete cutRect;
        cutRect = NULL;
    }
}

void MainWindow::on_actionrotate_triggered()
{
    currentState = myrotate;
    if(cutRect != NULL) {
        delete cutRect;
        cutRect = NULL;
    }
}

void MainWindow::on_actionzoom_triggered()
{
    currentState = zoom;
    if(cutRect != NULL) {
        delete cutRect;
        cutRect = NULL;
    }
}

void MainWindow::on_actionmakeFull_triggered()
{
    currentState = makeFull;
    selectedShape.isSelected = false;
    if(cutRect != NULL) {
        delete cutRect;
        cutRect = NULL;
    }
}

void MainWindow::on_actioncut_triggered()
{
    currentState = cut;
    if(cutRect != NULL) {
        delete cutRect;
        cutRect = NULL;
    }
}
