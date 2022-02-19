#include "mainwindow.h"
#include <qgridlayout>
#include <qlineedit>
#include <qitemdelegate>
#include <qmessagebox>
#include <qheaderview>
#include <qlocale>
#include "doubledelegate.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setCentralWidget(new QWidget(this));

    labelScale = new QLabel("Текущий масштаб:\n1 ячейка = 1 ед.", this);

    QLabel *labelSmallTriangle = new QLabel("Зелёный треугольник", this);
    QLabel *labelBigTriangle = new QLabel("Красный треугольник", this);
    QLabel *labelAnswer = new QLabel("Разница площадей", this);

    outBigTriangle = new QLineEdit(this);
    outSmallTriangle = new QLineEdit(this);
    outAnswer = new QLineEdit(this);
    outBigTriangle->setReadOnly(true);
    outSmallTriangle->setReadOnly(true);
    outAnswer->setReadOnly(true);
    setNewAnswer(-1, -1);

    buttonAddPoint = new QPushButton("Добавить точку", this);
    buttonRemovePoint = new QPushButton("Удалить точку", this);
    buttonAddPoint->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    buttonRemovePoint->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(buttonAddPoint, &QPushButton::clicked, this, &MainWindow::buttonAddPointClicked);
    connect(buttonRemovePoint, &QPushButton::clicked, this, &MainWindow::buttonRemovePointClicked);

    spinRemovePoint = new QSpinBox(this);
    spinRemovePoint->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    spinRemovePoint->setMinimum(1);
    spinRemovePoint->setMaximum(5);

    pointsTable = new QTableWidget(this);
    pointsTable->setItemDelegate(new DoubleDelegate(this));
    pointsTable->setFixedWidth(280);
    pointsTable->horizontalHeader()->setDefaultSectionSize(130);
    pointsTable->horizontalHeader()->setStretchLastSection(true);
    pointsTable->setColumnCount(2);
    pointsTable->setRowCount(5);
    for (int i = 0; i < 5; i++) {
        pointsTable->setItem(i, 0, new QTableWidgetItem("0"));
        pointsTable->setItem(i, 1, new QTableWidgetItem("0"));
    }
    pointsTable->setHorizontalHeaderLabels({ "X", "Y" });
    pointsTable->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    canvas = new Canvas(this);
    canvas->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    canvas->setMinimumSize(750, 600);
    canvas->setPointsTable(pointsTable);
    connect(pointsTable, &QTableWidget::cellChanged, canvas, [this]() { canvas->pointsTableChanged(); });
    connect(canvas, &Canvas::scaleChanged, this, &MainWindow::updateScale);
    connect(canvas, &Canvas::newAnswer, this, &MainWindow::setNewAnswer);
    
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(labelScale, 0, 0, 1, 2);
    layout->addWidget(buttonAddPoint, 1, 0, 1, 2);
    layout->addWidget(spinRemovePoint, 2, 0);
    layout->addWidget(buttonRemovePoint, 2, 1);
    layout->addWidget(outBigTriangle, 3, 1);
    layout->addWidget(outSmallTriangle, 4, 1);
    layout->addWidget(outAnswer, 5, 1);
    layout->addWidget(labelBigTriangle, 3, 0);
    layout->addWidget(labelSmallTriangle, 4, 0);
    layout->addWidget(labelAnswer, 5, 0);
    layout->addWidget(pointsTable, 6, 0, 1, 2);
    layout->addWidget(canvas, 0, 2, 7, 1);

    centralWidget()->setLayout(layout);
}

void MainWindow::buttonAddPointClicked() {
    pointsTable->insertRow(0);
    pointsTable->setItem(0, 0, new QTableWidgetItem("0"));
    pointsTable->setItem(0, 1, new QTableWidgetItem("0"));
    spinRemovePoint->setMaximum(spinRemovePoint->maximum() + 1);
    canvas->pointsTableChanged();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    canvas->windowResized();
}

void MainWindow::buttonRemovePointClicked() {
    if (pointsTable->rowCount() > 3) {
        pointsTable->removeRow(spinRemovePoint->value() - 1);
        spinRemovePoint->setMaximum(spinRemovePoint->maximum() - 1);
        canvas->pointsTableChanged();
    }
    else {
        QMessageBox::critical(this, "Ошибка", "Для выполнения задачи необходимо как минимум 3 точки");
    }
}

void MainWindow::updateScale(double scale) {
    labelScale->setText(QString("Текущий масштаб:\n1 ячейка = %1 ед.").arg(scale));
}

void MainWindow::setNewAnswer(double smallTriag, double bigTriag) {
    if (smallTriag < 0) {
        outAnswer->setText("N/A");
        outBigTriangle->setText("N/A");
        outSmallTriangle->setText("N/A");
    }
    else {
        outAnswer->setText(QString::number(bigTriag - smallTriag));
        outBigTriangle->setText(QString::number(bigTriag));
        outSmallTriangle->setText(QString::number(smallTriag));
    }
}
