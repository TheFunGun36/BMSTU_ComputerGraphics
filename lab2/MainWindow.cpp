#include "mainwindow.h"
#include <qgridlayout.h>
#include <qlineedit.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setCentralWidget(new QWidget(this));

    canvas = new Canvas(this);
    canvas->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    canvas->setMinimumSize(750, 600);
    
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(canvas, 0, 0, 1, 1);

    centralWidget()->setLayout(layout);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    canvas->windowResized();
}
