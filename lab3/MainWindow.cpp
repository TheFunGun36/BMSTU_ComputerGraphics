#include "MainWindow.h"
#include <qvalidator.h>
#include <qmessagebox.h>
#include <qchartview.h>
#include <qlineseries.h>
#include <qbarseries.h>
#include <qbarset.h>
#include <qbarcategoryaxis.h>
#include <qvalueaxis.h>
#include "LineAlgorithm.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    ui.setupUi(this);

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

void MainWindow::setupTabGraph() {
    QGridLayout *layout = new QGridLayout(ui.tabLadder);
    QLabel *label = new QLabel("Анализ количества ступенек для отрезка длины 100 пикселей\nОсь X - угол, ось Y - количество ступенек", this);
    label->setAlignment(Qt::AlignCenter);

    QChartView *chartDDA = new QChartView(this);
    QChartView *chartWu = new QChartView(this);
    QChartView *chartBrezenhamInt = new QChartView(this);
    QChartView *chartBrezenhamFloat = new QChartView(this);
    QChartView *chartBrezenhamAntialias = new QChartView(this);

    QLineSeries *sDDA = new QLineSeries();
    QLineSeries *sWu = new QLineSeries();
    QLineSeries *sBrezenhamInt = new QLineSeries();
    QLineSeries *sBrezenhamFloat = new QLineSeries();
    QLineSeries *sBrezenhamAntialias = new QLineSeries();

    int xSteps = 360;
    qreal angle = 2 * M_PI / xSteps;
    QLineF line(0., 0., 100., 0.);
    qreal dx = line.p2().x();
    qreal dy = line.p2().y();

    for (int i = 0; i <= xSteps; i++) {
        qreal cosa = qCos(i * angle);
        qreal sina = qSin(i * angle);
        qreal x = dx * cosa + dy * sina;
        qreal y = dx * sina + dy * cosa;
        line.setP2(QPointF(x, y));

        int value = countLaddersDDA(line.toLine());
        sDDA->append(i, value);
        value = countLaddersWu(line.toLine());
        sWu->append(i, value);
        value = countLaddersBrezenhamInt(line.toLine());
        sBrezenhamInt->append(i, value);
        value = countLaddersBrezenhamFloat(line.toLine());
        sBrezenhamFloat->append(i, value);
        value = countLaddersBrezenhamAntialias(line.toLine());
        sBrezenhamAntialias->append(i, value);
    }

    chartDDA->chart()->addSeries(sDDA);
    chartDDA->chart()->setTitle("Цифровой дифференциальный анализатор");
    chartDDA->chart()->createDefaultAxes();
    chartDDA->chart()->legend()->hide();

    chartWu->chart()->addSeries(sWu);
    chartWu->chart()->setTitle("Алгоритм Ву");
    chartWu->chart()->createDefaultAxes();
    chartWu->chart()->legend()->hide();

    chartBrezenhamInt->chart()->addSeries(sBrezenhamInt);
    chartBrezenhamInt->chart()->setTitle("Алгоритм Брезенхема (целочисленный)");
    chartBrezenhamInt->chart()->createDefaultAxes();
    chartBrezenhamInt->chart()->legend()->hide();

    chartBrezenhamFloat->chart()->addSeries(sBrezenhamFloat);
    chartBrezenhamFloat->chart()->setTitle("Алгоритм Брезенхема (вещественный)");
    chartBrezenhamFloat->chart()->createDefaultAxes();
    chartBrezenhamFloat->chart()->legend()->hide();

    chartBrezenhamAntialias->chart()->addSeries(sBrezenhamAntialias);
    chartBrezenhamAntialias->chart()->setTitle("Алгоритм Брезенхема (со сглаживанием)");
    chartBrezenhamAntialias->chart()->createDefaultAxes();
    chartBrezenhamAntialias->chart()->legend()->hide();

    layout->addWidget(label, 0, 0, 1, 2);
    layout->addWidget(chartBrezenhamFloat, 1, 0);
    layout->addWidget(chartBrezenhamInt, 2, 0);
    layout->addWidget(chartBrezenhamAntialias, 3, 0);
    layout->addWidget(chartDDA, 1, 1);
    layout->addWidget(chartWu, 2, 1);
}

void MainWindow::setupTabChart() {
    QVBoxLayout *layout = new QVBoxLayout(ui.tabChart);
    QLabel *label = new QLabel("Анализ производительности алгоритмов для отрезка длиной 100 пикселей, 1000 прогонов\nна оси Y время в наносекундах", this);
    label->setAlignment(Qt::AlignCenter);

    QChartView *chart = new QChartView(this);
    barSeries = new QBarSeries(this);

    QPushButton *buttonTest = new QPushButton("Рассчитать", this);
    buttonTest->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    
    QStringList categories;
    categories << "Цифровой дифференциальный анализатор"
        << "Алгоритм Ву"
        << "Алгоритм Брезенхема (целочисленный)"
        << "Алгоритм Брезенхема (вещественный)"
        << "Алгоритм Брезенхема (со сглаживанием)";

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->chart()->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    axisY = new QValueAxis();
    chart->chart()->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    chart->chart()->legend()->hide();

    QBarSet *barSet = new QBarSet("abc", this);
    for (int i = 0; i < 5; i++)
        barSet->append(80000000);
    barSeries->append(barSet);

    axisY->setRange(0, 80000000);


    chart->chart()->addSeries(barSeries);

    connect(buttonTest, &QPushButton::clicked, this, [this]() {
        long long test[5];
        test[0] = testAlgorithm(1000, 100, algorithmDDA);
        test[1] = testAlgorithm(1000, 100, algorithmWu);
        test[2] = testAlgorithm(1000, 100, algorithmBrezenhamInt);
        test[3] = testAlgorithm(1000, 100, algorithmBrezenhamFloat);
        test[4] = testAlgorithm(1000, 100, algorithmBrezenhamAntialias);

        barSeries->clear();
        QBarSet *barSet = new QBarSet("abs", this);
        for (int i = 0; i < 5; i++)
            barSet->append(test[i]);
        barSeries->append(barSet);
        });

    layout->addWidget(label);
    layout->addWidget(buttonTest, Qt::AlignCenter);
    layout->addWidget(chart);
}
