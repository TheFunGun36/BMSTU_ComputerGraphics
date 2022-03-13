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
    QIntValidator *validatorInt = new QIntValidator(this);
    validatorInt->setLocale(QLocale::C);
    ui.drawLineStartX->setValidator(validatorInt);
    ui.drawLineStartY->setValidator(validatorInt);
    ui.drawLineEndX->setValidator(validatorInt);
    ui.drawLineEndY->setValidator(validatorInt);

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
                ui.drawCanvas->addSun(QLine(p1, p2), clr, alg);
            }
        });

    connect(ui.drawButtonUndo, &QPushButton::clicked, ui.drawCanvas, &Canvas::undo);
    connect(ui.drawButtonClear, &QPushButton::clicked, ui.drawCanvas, &Canvas::clear);
}

void MainWindow::setupTabSun() {
    QIntValidator *validatorInt = new QIntValidator(this);
    validatorInt->setLocale(QLocale::C);
    QDoubleValidator *validatorDouble = new QDoubleValidator(this);
    validatorDouble->setLocale(QLocale::C);

    ui.sunLineCenterX->setValidator(validatorInt);
    ui.sunLineCenterY->setValidator(validatorInt);
    ui.sunLineAmount->setValidator(validatorInt);
    ui.sunLineLength->setValidator(validatorDouble);

    connect(ui.sunButtonPaint, &QPushButton::clicked, this,
        [this]() {
            bool ok = true;
            QPoint p1;
            qreal length;
            int amount;
            if (ok) p1.setX(ui.sunLineCenterX->text().toInt(&ok));
            if (ok) p1.setY(ui.sunLineCenterY->text().toInt(&ok));
            if (ok) length = ui.sunLineLength->text().toDouble(&ok);
            if (ok) amount = ui.sunLineAmount->text().toInt(&ok);
            if (!ok) QMessageBox::warning(this, "Ошибка", "Не все параметры \"солнца\" были введены");
            else if (length <= 1.0) QMessageBox::warning(this, "Ошибка", "Длина отрезков должна быть больше единицы");
            else if (amount < 1) QMessageBox::warning(this, "Ошибка", "Количество отрезков должно быть натуральным числом");
            else {
                LineColor clr = static_cast<LineColor>(ui.sunComboColor->currentIndex());
                LineAlgorithm alg = static_cast<LineAlgorithm>(ui.sunComboAlgorithm->currentIndex());
                ui.sunCanvas->addSun(QLine(p1, QPoint(p1.x() + length, p1.y())), clr, alg, amount);
            }
        });
    connect(ui.sunButtonUndo, &QPushButton::clicked, ui.sunCanvas, &Canvas::undo);
    connect(ui.sunButtonClear, &QPushButton::clicked, ui.sunCanvas, &Canvas::clear);
}

void MainWindow::setupTabGraph() {}

void MainWindow::setupTabChart() {}
