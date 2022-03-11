#include "MainWindow.h"
#include <qvalidator.h>
#include <qmessagebox.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    ui.setupUi(this);
    setFixedHeight(height());
    setFixedWidth(width());

    setupTabDraw();
    setupTabSun();
    setupTabGraph();
    setupTabChart();
}

void MainWindow::setupTabDraw() {
    QIntValidator *validator = new QIntValidator(this);
    ui.drawLineStartX->setValidator(validator);
    ui.drawLineStartY->setValidator(validator);
    ui.drawLineEndX->setValidator(validator);
    ui.drawLineEndY->setValidator(validator);

    connect(ui.drawButtonPaint, &QPushButton::clicked, this,
        [this]() {
            bool ok = true;
            QPoint p1;
            QPoint p2;
            if (ok) p1.setX(ui.drawLineStartX->text().toInt(&ok));
            if (ok) p1.setY(ui.drawLineStartY->text().toInt(&ok));
            if (ok) p2.setX(ui.drawLineEndX->text().toInt(&ok));
            if (ok) p2.setY(ui.drawLineEndY->text().toInt(&ok));
            if (!ok) QMessageBox::warning(this, "Ошибка", "Не все значения координат отрезка были введены");
            else {
                LineColor clr = static_cast<LineColor>(ui.drawComboColor->currentIndex());
                LineAlgorithm alg = static_cast<LineAlgorithm>(ui.drawComboAlgorithm->currentIndex());
                ui.drawCanvas->addLine(QLine(p1, p2), clr, alg);
            }
        });

    connect(ui.drawButtonUndo, &QPushButton::clicked, ui.drawCanvas, &Canvas::undo);
    connect(ui.drawButtonClear, &QPushButton::clicked, ui.drawCanvas, &Canvas::clear);
}

void MainWindow::setupTabSun() {}

void MainWindow::setupTabGraph() {}

void MainWindow::setupTabChart() {}
