#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qdebug.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->buttonClear, &QPushButton::clicked, ui->canvas, &Canvas::clear);
    connect(ui->speed, &QSlider::valueChanged, ui->canvas, &Canvas::changeFillSpeed);
    connect(ui->checkSeparator, &QCheckBox::toggled, ui->canvas, &Canvas::setSeparatorDraw);
    connect(ui->checkRect, &QCheckBox::toggled, ui->canvas, &Canvas::setRectDraw);
    connect(ui->buttonFill, &QPushButton::clicked, ui->canvas, &Canvas::beginFill);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onFillClicked() {
    QColor clr;
    switch (ui->color->currentIndex()) {
    case 0: clr = QColor(Qt::black); break;
    case 1: clr = QColor(Qt::red); break;
    case 2: clr = QColor(Qt::green); break;
    case 3: clr = QColor(Qt::blue); break;
    case 4: clr = QColor(Qt::white); break;
    }
}
