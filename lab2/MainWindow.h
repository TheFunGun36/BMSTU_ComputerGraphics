#pragma once
#include "canvas.h"
#include <qmainwindow.h>
#include <qlayout.h>
#include <qlineedit.h>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    Canvas *canvas;

    QLineEdit *lineMoveDx;
    QLineEdit *lineMoveDy;

    QLineEdit *lineScaleKx;
    QLineEdit *lineScaleKy;
    QLineEdit *lineScaleCenterX;
    QLineEdit *lineScaleCenterY;

    QLineEdit *lineRotateAngle;
    QLineEdit *lineRotateCenterX;
    QLineEdit *lineRotateCenterY;

    void createMoveSection(QVBoxLayout *layout);
    void createScaleSection(QVBoxLayout *layout);
    void createRotateSection(QVBoxLayout *layout);

private slots:
    void clickedScaleRelativeToObject();
    void clickedScaleRelativeToOrigin();
    void clickedRotateRelativeToObject();
    void clickedRotateRelativeToOrigin();
    void clickedApplyMove();
    void clickedApplyScale();
    void clickedApplyRotate();
    void clickedUndo();
};
