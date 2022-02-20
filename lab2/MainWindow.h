#pragma once
#include "canvas.h"
#include <qmainwindow.h>
#include <qlayout.h>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    void createMoveSection(QVBoxLayout *layout);
    void createScaleSection(QVBoxLayout *layout);
    void createRotateSection(QVBoxLayout *layout);
    Canvas *canvas;
};
