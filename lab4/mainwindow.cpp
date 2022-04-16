#include "mainwindow.h"
#include "canvas.h"
#include "qcustomplot.h"
#include "algorithm.h"
#include <qvalidator.h>
#include <qmessagebox.h>
#include <qprogressbar.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    ui.setupUi(this);

    setupTabDraw();
    setupTabSpectrum();
    setupTabTest();
}

void MainWindow::setupTabDraw() {
    QIntValidator *validatorInt = new QIntValidator(this);
    ui.drawLineCenterX->setValidator(validatorInt);
    ui.drawLineCenterY->setValidator(validatorInt);
    ui.drawLineSizeX->setValidator(validatorInt);
    ui.drawLineSizeY->setValidator(validatorInt);
    ui.drawLineRadius->setValidator(validatorInt);

    connect(ui.drawButtonPaintEllipse, &QPushButton::clicked, this,
        [this]() {
            bool ok = true;
            Ellipse e;
            if (ok) e.cx = ui.drawLineCenterX->text().toInt(&ok);
            if (ok) e.cy = ui.drawLineCenterY->text().toInt(&ok);
            if (ok) e.rx = ui.drawLineSizeX->text().toInt(&ok);
            if (ok) e.ry = ui.drawLineSizeY->text().toInt(&ok);
            if (!ok) QMessageBox::warning(this, "Ошибка", "Не все значения эллипса были введены");
            else {
                Color clr = Color(ui.drawComboColor->currentIndex());
                Algorithm alg = Algorithm(ui.drawComboAlgorithm->currentIndex());
                ui.drawCanvas->addEllipse(e, clr, alg);
            }
        });

    connect(ui.drawButtonPaintCircle, &QPushButton::clicked, this,
        [this]() {
            bool ok = true;
            Ellipse e;
            e.ry = 0.;
            if (ok) e.cx = ui.drawLineCenterX->text().toInt(&ok);
            if (ok) e.cy = ui.drawLineCenterY->text().toInt(&ok);
            if (ok) e.rx = ui.drawLineRadius->text().toInt(&ok);
            if (!ok) QMessageBox::warning(this, "Ошибка", "Не все значения окружности были введены");
            else {
                Color clr = Color(ui.drawComboColor->currentIndex());
                Algorithm alg = Algorithm(ui.drawComboAlgorithm->currentIndex());
                ui.drawCanvas->addCircle(e, clr, alg);
            }
        });

    connect(ui.drawButtonClear, &QPushButton::clicked, ui.drawCanvas, &Canvas::clear);
}

void MainWindow::setupTabSpectrum() {
    QIntValidator *validatorInt = new QIntValidator(this);
    ui.spectrumLineBeginX->setValidator(validatorInt);
    ui.spectrumLineBeginY->setValidator(validatorInt);
    ui.spectrumLineEndX->setValidator(validatorInt);
    ui.spectrumLineEndY->setValidator(validatorInt);
    ui.spectrumLineRadiusBegin->setValidator(validatorInt);
    ui.spectrumLineRadiusEnd->setValidator(validatorInt);

    connect(ui.spectrumButtonPaintEllipse, &QPushButton::clicked, this,
        [this]() {
            bool ok = true;
            Ellipse begin;
            begin.cx = 350;
            begin.cy = 350;

            Ellipse end;
            end.cx = 350;
            end.cy = 350;

            int amount = 0;
            if (ok) begin.rx = ui.spectrumLineBeginX->text().toInt(&ok);
            if (ok) begin.ry = ui.spectrumLineBeginY->text().toInt(&ok);
            if (ok) end.rx = ui.spectrumLineEndX->text().toInt(&ok);
            if (ok) end.ry = ui.spectrumLineEndY->text().toInt(&ok);
            if (ok) amount = ui.spectrumLineAmount->text().toInt(&ok);
            if (!ok) QMessageBox::warning(this, "Ошибка", "Не все параметры спектра были введены");
            else if (begin.rx == 0 || begin.ry == 0 || end.rx == 0 || end.ry == 0) {
                QMessageBox::warning(this, "Ошибка", "Радиус должен быть положительным числом");
            }
            else if (amount < 2) {
                QMessageBox::warning(this, "Ошибка", "Количество эллипсов должно быть не меньше двух");
            }
            else {
                Color clr = Color(ui.spectrumComboColor->currentIndex());
                Algorithm alg = Algorithm(ui.spectrumComboAlgorithm->currentIndex());
                ui.spectrumCanvas->addEllipseSpectrum(begin, end, amount, clr, alg);
            }
        });

    connect(ui.spectrumButtonPaintCircle, &QPushButton::clicked, this,
        [this]() {
            bool ok = true;
            Ellipse begin;
            begin.cx = 350;
            begin.cy = 350;
            begin.ry = 0;

            Ellipse end;
            end.cx = 350;
            end.cy = 350;
            end.ry = 0;

            int amount = 0;
            if (ok) begin.rx = ui.spectrumLineRadiusBegin->text().toInt(&ok);
            if (ok) end.rx = ui.spectrumLineRadiusEnd->text().toInt(&ok);
            if (ok) amount = ui.spectrumLineAmount->text().toInt(&ok);
            if (!ok) QMessageBox::warning(this, "Ошибка", "Не все параметры спектра были введены");
            else if (begin.rx == 0 || end.rx == 0) {
                QMessageBox::warning(this, "Ошибка", "Радиус должен быть положительным числом");
            }
            else if (amount < 2) {
                QMessageBox::warning(this, "Ошибка", "Количество окружностей должно быть не меньше двух");
            }
            else {
                Color clr = Color(ui.spectrumComboColor->currentIndex());
                Algorithm alg = Algorithm(ui.spectrumComboAlgorithm->currentIndex());
                ui.spectrumCanvas->addCircleSpectrum(begin, end, amount, clr, alg);
            }
        });

    connect(ui.spectrumButtonClear, &QPushButton::clicked, ui.spectrumCanvas, &Canvas::clear);
}

void MainWindow::setupTabTest() {
    //*
    //ui.testPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    graphParam = ui.testPlot->addGraph();
    graphCanon = ui.testPlot->addGraph();
    graphBrezenham = ui.testPlot->addGraph();
    graphAvgPoint = ui.testPlot->addGraph();
    //QVector<double> x(n), y(n);
    //for (int i=0; i<n; ++i) {
    //  x[i] = i/(double)(n-1)*34 - 17;
    //  y[i] = qExp(-x[i]*x[i]/20.0)*qSin(k*x[i]+phase);
    //}
    //graph->setData(x, y);
    graphParam->setPen(QPen(Qt::black));
    graphParam->setName("Параметрические уравнения");

    graphCanon->setPen(QPen(Qt::red));
    graphCanon->setName("Канонические уравнения");

    graphBrezenham->setPen(QPen(Qt::green));
    graphBrezenham->setName("Алгоритм Брезенхема");

    graphAvgPoint->setPen(QPen(Qt::blue));
    graphAvgPoint->setName("Алгоритм средней точки");
    ui.testPlot->legend->setVisible(true);
    ui.testPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);

    connect(ui.testButtonEllipse, &QPushButton::clicked, this, [this] () {
        const size_t points = 200;

        QVector<double> x(points);
        QVector<double> y(points);
        const size_t iterationsPerPoint = 50;
        const size_t maxDelta = 5000;
        const size_t triesMax = 10;

        for (size_t i = 0; i < points; i++) {
            ui.testProgress->setValue(i * 100 / points / 4);
            x[i] = i * 4 + 200;
            size_t tries = 0;
            do
                y[i] = testAlgorithmEllipse(algParametricalEq, iterationsPerPoint, x[i]);
            while (i > 0 && tries++ < triesMax && y[i] - y[i - 1] > maxDelta);
        }
        graphParam->setData(x, y);

        for (size_t i = 0; i < points; i++) {
            ui.testProgress->setValue(25 + i * 100 / points / 4);
            size_t tries = 0;
            do
                y[i] = testAlgorithmEllipse(algCanonicalEq, iterationsPerPoint, x[i]);
            while (i > 0 && tries++ < triesMax && y[i] - y[i - 1] > maxDelta);
        }
        graphCanon->setData(x, y);

        for (size_t i = 0; i < points; i++) {
            ui.testProgress->setValue(50 + i * 100 / points / 4);
            size_t tries = 0;
            do
                y[i] = testAlgorithmEllipse(algAveragePoint, iterationsPerPoint, x[i]);
            while (i > 0 && tries++ < triesMax && y[i] - y[i - 1] > maxDelta);
        }
        graphAvgPoint->setData(x, y);

        for (size_t i = 0; i < points; i++) {
            ui.testProgress->setValue(75 + i * 100 / points / 4);
            size_t tries = 0;
            do
                y[i] = testAlgorithmEllipse(algBrezenham, iterationsPerPoint, x[i]);
            while (i > 0 && tries++ < triesMax && y[i] - y[i - 1] > maxDelta);
        }
        graphBrezenham->setData(x, y);

        ui.testProgress->setValue(100);
        ui.testPlot->rescaleAxes();
        ui.testPlot->replot();
    });

    connect(ui.testButtonCircle, &QPushButton::clicked, this, [this] () {
        const size_t points = 200;

        QVector<double> x(points);
        QVector<double> y(points);
        const size_t iterationsPerPoint = 50;
        const size_t maxDelta = 5000;
        const size_t triesMax = 10;

        for (size_t i = 0; i < points; i++) {
            ui.testProgress->setValue(i * 100 / points / 4);
            x[i] = i * 4 + 200;
            size_t tries = 0;
            do
                y[i] = testAlgorithmCircle(algParametricalEq, iterationsPerPoint, x[i]);
            while (i > 0 && tries++ < triesMax && y[i] - y[i - 1] > maxDelta);
        }
        graphParam->setData(x, y);

        for (size_t i = 0; i < points; i++) {
            ui.testProgress->setValue(25 + i * 100 / points / 4);
            size_t tries = 0;
            do
                y[i] = testAlgorithmCircle(algCanonicalEq, iterationsPerPoint, x[i]);
            while (i > 0 && tries++ < triesMax && y[i] - y[i - 1] > maxDelta);
        }
        graphCanon->setData(x, y);

        for (size_t i = 0; i < points; i++) {
            ui.testProgress->setValue(50 + i * 100 / points / 4);
            size_t tries = 0;
            do
                y[i] = testAlgorithmCircle(algAveragePoint, iterationsPerPoint, x[i]);
            while (i > 0 && tries++ < triesMax && y[i] - y[i - 1] > maxDelta);
        }
        graphAvgPoint->setData(x, y);

        for (size_t i = 0; i < points; i++) {
            ui.testProgress->setValue(75 + i * 100 / points / 4);
            size_t tries = 0;
            do
                y[i] = testAlgorithmCircle(algBrezenham, iterationsPerPoint, x[i]);
            while (i > 0 && tries++ < triesMax && y[i] - y[i - 1] > maxDelta);
        }
        graphBrezenham->setData(x, y);

        ui.testProgress->setValue(100);
        ui.testPlot->rescaleAxes();
        ui.testPlot->replot();
    });
    //*/
}
