#pragma once
#include "canvas.h"
#include <qmainwindow.h>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    Canvas *canvas;
    void resizeEvent(QResizeEvent *event);
};
