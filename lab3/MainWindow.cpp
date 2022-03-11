#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    ui.setupUi(this);
    setFixedHeight(height());
    setFixedWidth(width());
}
