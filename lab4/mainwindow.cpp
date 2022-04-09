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

    connect(ui.drawButtonPaint, &QPushButton::clicked, this,
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

    connect(ui.drawButtonClear, &QPushButton::clicked, ui.drawCanvas, &Canvas::clear);
}

void MainWindow::setupTabSpectrum() {
    QIntValidator *validatorInt = new QIntValidator(this);
    ui.spectrumLineBeginX->setValidator(validatorInt);
    ui.spectrumLineBeginY->setValidator(validatorInt);
    ui.spectrumLineEndX->setValidator(validatorInt);
    ui.spectrumLineEndY->setValidator(validatorInt);

    connect(ui.spectrumButtonPaint, &QPushButton::clicked, this,
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
                ui.spectrumCanvas->addSpectrum(begin, end, amount, clr, alg);
            }
        });

    connect(ui.spectrumButtonClear, &QPushButton::clicked, ui.spectrumCanvas, &Canvas::clear);
}

void MainWindow::setupTabTest() {
    // set dark background gradient:
    QLinearGradient gradient(0, 0, 0, 400);
    gradient.setColorAt(0, QColor(90, 90, 90));
    gradient.setColorAt(0.38, QColor(105, 105, 105));
    gradient.setColorAt(1, QColor(70, 70, 70));
    ui.testPlot->setBackground(QBrush(gradient));

    // create empty bar chart objects
    chartBars = new QCPBars(ui.testPlot->xAxis, ui.testPlot->yAxis);
    chartBars->setAntialiased(false); // gives more crisp, pixel aligned bar borders
    chartBars->setStackingGap(1);

    // set names and colors
    chartBars->setName("Время в микросекундах, затраченное на один прогон\n(среднее за 3000 прогонов)");
    chartBars->setPen(QPen(QColor(111, 9, 176).lighter(170)));
    chartBars->setBrush(QColor(111, 9, 176));

    // stack bars on top of each other
    //nuclear->moveAbove(fossil);
    //regen->moveAbove(nuclear);

    // prepare x axis with country labels
    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 1 << 2 << 3 << 4;
    labels << "Параметрические ур-я" << "Каноническое ур-е" << "Алгоритм брезенхема" << "Алгоритм средней точки";

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    ui.testPlot->xAxis->setTicker(textTicker);
    ui.testPlot->xAxis->setTickLabelRotation(10);
    ui.testPlot->xAxis->setSubTicks(false);
    ui.testPlot->xAxis->setTickLength(0, 4);
    ui.testPlot->xAxis->setRange(0, 6);
    ui.testPlot->xAxis->setBasePen(QPen(Qt::white));
    ui.testPlot->xAxis->setTickPen(QPen(Qt::white));
    ui.testPlot->xAxis->grid()->setVisible(true);
    ui.testPlot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    ui.testPlot->xAxis->setTickLabelColor(Qt::white);
    ui.testPlot->xAxis->setLabelColor(Qt::white);

    // prepare y axis:
    ui.testPlot->yAxis->setRange(0, 10);
    ui.testPlot->yAxis->setPadding(5); // a bit more space to the left border
    ui.testPlot->yAxis->setBasePen(QPen(Qt::white));
    ui.testPlot->yAxis->setTickPen(QPen(Qt::white));
    ui.testPlot->yAxis->setSubTickPen(QPen(Qt::white));
    ui.testPlot->yAxis->grid()->setSubGridVisible(true);
    ui.testPlot->yAxis->setTickLabelColor(Qt::white);
    ui.testPlot->yAxis->setLabelColor(Qt::white);
    ui.testPlot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
    ui.testPlot->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

    // Add data:
    QVector<double> data;
    data << 0 << 0 << 0 << 0;
    chartBars->setData(ticks, data);

    // setup legend:
    ui.testPlot->legend->setVisible(true);
    ui.testPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    ui.testPlot->legend->setBrush(QColor(255, 255, 255, 100));
    ui.testPlot->legend->setBorderPen(Qt::NoPen);

    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui.testPlot->legend->setFont(legendFont);
    ui.testPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    connect(ui.testButton, &QPushButton::clicked, this, [this] () {
        const int iter = 3000;

        ui.testProgress->setValue(10);
        double v4 = testAlgorithm(algAveragePoint, iter);
        ui.testProgress->setValue(25);
        double v3 = testAlgorithm(algBrezenham, iter);
        ui.testProgress->setValue(50);
        double v2 = testAlgorithm(algCanonicalEq, iter);
        ui.testProgress->setValue(75);
        double v1 = testAlgorithm(algParametricalEq, iter);
        ui.testProgress->setValue(100);

        double m = qMax(qMax(v1, v2), qMax(v3, v4));
        chartBars->setData({1, 2, 3, 4}, {v1, v2, v3, v4});
        ui.testPlot->yAxis->setRange(0, m);
        ui.testPlot->replot();
    });
}
