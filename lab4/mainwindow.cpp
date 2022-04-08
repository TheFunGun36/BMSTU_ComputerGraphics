#include "mainwindow.h"
#include "canvas.h"
#include <qvalidator.h>
#include <qmessagebox.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    ui.setupUi(this);

    setupTabDraw();
    setupTabSpectrum();
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
