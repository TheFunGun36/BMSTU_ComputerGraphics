#pragma once

#include <QtWidgets/QMainWindow>
#include <qbarset.h>
#include <qbarseries.h>
#include <qvalueaxis.h>
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

private:
    QBarSeries *barSeries;
    QValueAxis *axisY;
    Ui::MainWindowClass ui;
    void setupTabDraw();
    void setupTabSun();
    void setupTabGraph();
    void setupTabChart();
};
