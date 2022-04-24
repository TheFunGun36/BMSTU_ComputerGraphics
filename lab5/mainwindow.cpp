#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qmessagebox.h>
#include <qdebug.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->buttonClear, &QPushButton::clicked, ui->canvas, &Canvas::clear);
    connect(ui->speed, &QSlider::valueChanged, ui->canvas, &Canvas::changeFillSpeed);
    connect(ui->checkSeparator, &QCheckBox::toggled, ui->canvas, &Canvas::setSeparatorDraw);
    connect(ui->checkRect, &QCheckBox::toggled, ui->canvas, &Canvas::setRectDraw);
    connect(ui->checkLines, &QCheckBox::toggled, ui->canvas, &Canvas::setDrawLines);
    connect(ui->buttonFill, &QPushButton::clicked, this, &MainWindow::onFillClicked);
    connect(ui->buttonTest, &QPushButton::clicked, this, &MainWindow::onTestClicked);
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
    ui->canvas->setColorFill(clr);
    ui->canvas->beginFill();
}

void MainWindow::onTestClicked() {
    try {
        auto time = ui->canvas->testAlgorithm();
        ui->lineTestResult->setText(QString("%0 ms").arg(time.count()));
    }
    catch (const char *str) {
        QMessageBox::warning(this, "Ошибка", "Поле пусто");
    }
}
