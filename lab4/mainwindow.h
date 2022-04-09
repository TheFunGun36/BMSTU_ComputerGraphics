#pragma once
#include <qmainwindow.h>
#include "ui_mainwindow.h"
#include "qcustomplot.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    Ui::MainWindowClass ui;
    void setupTabDraw();
    void setupTabSpectrum();
    void setupTabTest();

    QCPBars *chartBars;
};
