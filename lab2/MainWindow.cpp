#include "mainwindow.h"
#include <qlabel.h>
#include <qpushbutton.h>
#include <qvalidator.h>
#include <qmessagebox.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setCentralWidget(new QWidget(this));

    canvas = new Canvas(this);
    canvas->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    canvas->setMinimumSize(750, 600);

    auto layout = new QHBoxLayout(this);
    auto leftPanel = new QVBoxLayout(this);

    centralWidget()->setLayout(layout);

    setLocale(QLocale("C"));
    createMoveSection(leftPanel);
    leftPanel->addSpacerItem(new QSpacerItem(0, 20));
    createScaleSection(leftPanel);
    leftPanel->addSpacerItem(new QSpacerItem(0, 20));
    createRotateSection(leftPanel);
    leftPanel->addSpacerItem(new QSpacerItem(0, 20));

    QPushButton *cancel = new QPushButton("Отменить последнее действие", this);
    cancel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(cancel, &QPushButton::clicked, this, &MainWindow::clickedUndo);
    leftPanel->addWidget(cancel);

    QPushButton *reset = new QPushButton("Сбросить", this);
    reset->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(reset, &QPushButton::clicked, canvas, &Canvas::reset);
    leftPanel->addWidget(reset);

    QPushButton *goToFigure = new QPushButton("Переместиться к фигуре", this);
    goToFigure->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(goToFigure, &QPushButton::clicked, canvas, &Canvas::goToFigure);
    leftPanel->addWidget(goToFigure);

    leftPanel->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    layout->addLayout(leftPanel);
    layout->addWidget(canvas);
}

void MainWindow::createMoveSection(QVBoxLayout *layout) {
    QLabel *title = new QLabel("Перенос", this);
    title->setFont(QFont("Times New Roman", 18));
    layout->addWidget(title);

    lineMoveDx = new QLineEdit(this);
    lineMoveDy = new QLineEdit(this);
    lineMoveDx->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    lineMoveDy->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QIntValidator *validator = new QIntValidator(this);
    lineMoveDx->setValidator(validator);
    lineMoveDy->setValidator(validator);

    auto layoutData = new QGridLayout(this);
    layoutData->addWidget(new QLabel("Смещение по x:", this), 0, 0);
    layoutData->addWidget(new QLabel("Смещение по y:", this), 1, 0);
    layoutData->addWidget(lineMoveDx, 0, 1);
    layoutData->addWidget(lineMoveDy, 1, 1);

    layout->addLayout(layoutData);

    QPushButton *apply = new QPushButton("Перенести", this);
    apply->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(apply, &QPushButton::clicked, this, &MainWindow::clickedApplyMove);
    layout->addWidget(apply);
};

void MainWindow::createScaleSection(QVBoxLayout *layout) {
    QLabel *title = new QLabel("Масштаб", this);
    title->setFont(QFont("Times New Roman", 18));
    layout->addWidget(title);

    lineScaleKx = new QLineEdit(this);
    lineScaleKy = new QLineEdit(this);
    lineScaleCenterX = new QLineEdit(this);
    lineScaleCenterY = new QLineEdit(this);
    lineScaleKx->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    lineScaleKy->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    lineScaleCenterX->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    lineScaleCenterY->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    lineScaleCenterX->setPlaceholderText("x");
    lineScaleCenterY->setPlaceholderText("y");

    QIntValidator *validator = new QIntValidator(this);
    QDoubleValidator *validatorDouble = new QDoubleValidator(this);
    validatorDouble->setLocale(QLocale("C"));
    lineScaleKx->setValidator(validatorDouble);
    lineScaleKy->setValidator(validatorDouble);
    lineScaleCenterX->setValidator(validator);
    lineScaleCenterY->setValidator(validator);

    auto layoutData = new QGridLayout(this);
    layoutData->addWidget(new QLabel("Коэффициент x:", this), 0, 0);
    layoutData->addWidget(new QLabel("Коэффициент y:", this), 1, 0);
    layoutData->addWidget(lineScaleKx, 0, 1);
    layoutData->addWidget(lineScaleKy, 1, 1);

    layout->addLayout(layoutData);

    layout->addWidget(new QLabel("Центр масштабирования:", this));
    auto layoutCenter = new QHBoxLayout(this);
    layoutCenter->addWidget(lineScaleCenterX);
    layoutCenter->addWidget(lineScaleCenterY);
    layout->addLayout(layoutCenter);

    QPushButton *useObjCenter = new QPushButton("Относительно центра фигуры", this);
    connect(useObjCenter, &QPushButton::clicked, this, &MainWindow::clickedScaleRelativeToObject);
    QPushButton *useOrigin = new QPushButton("Относительно начала координат", this);
    connect(useOrigin, &QPushButton::clicked, this, &MainWindow::clickedScaleRelativeToOrigin);
    layout->addWidget(useObjCenter);
    layout->addWidget(useOrigin);

    QPushButton *apply = new QPushButton("Масштабировать", this);
    apply->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(apply, &QPushButton::clicked, this, &MainWindow::clickedApplyScale);
    layout->addWidget(apply);
};

void MainWindow::createRotateSection(QVBoxLayout *layout) {
    QLabel *title = new QLabel("Поворот", this);
    title->setFont(QFont("Times New Roman", 18));
    layout->addWidget(title);

    lineRotateAngle = new QLineEdit(this);
    lineRotateCenterX = new QLineEdit(this);
    lineRotateCenterY = new QLineEdit(this);
    lineRotateAngle->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    lineRotateCenterX->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    lineRotateCenterY->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    lineRotateCenterX->setPlaceholderText("x");
    lineRotateCenterY->setPlaceholderText("y");

    QIntValidator *validator = new QIntValidator(this);
    QDoubleValidator *validatorDouble = new QDoubleValidator(this);
    validatorDouble->setLocale(QLocale("C"));
    lineRotateAngle->setValidator(validatorDouble);
    lineRotateCenterX->setValidator(validator);
    lineRotateCenterY->setValidator(validator);

    layout->addWidget(new QLabel("Угол в градусах (против часовой стрелки):", this));
    layout->addWidget(lineRotateAngle);

    layout->addWidget(new QLabel("Центр поворота:", this));
    auto layoutCenter = new QHBoxLayout(this);
    layoutCenter->addWidget(lineRotateCenterX);
    layoutCenter->addWidget(lineRotateCenterY);
    layout->addLayout(layoutCenter);

    QPushButton *useObjCenter = new QPushButton("Относительно центра фигуры", this);
    connect(useObjCenter, &QPushButton::clicked, this, &MainWindow::clickedRotateRelativeToObject);
    QPushButton *useOrigin = new QPushButton("Относительно начала координат", this);
    connect(useOrigin, &QPushButton::clicked, this, &MainWindow::clickedRotateRelativeToOrigin);
    layout->addWidget(useObjCenter);
    layout->addWidget(useOrigin);

    QPushButton *apply = new QPushButton("Повернуть", this);
    apply->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(apply, &QPushButton::clicked, this, &MainWindow::clickedApplyRotate);
    layout->addWidget(apply);
};

void MainWindow::clickedScaleRelativeToObject() {
    QPoint p = canvas->getFigureCenter();
    lineScaleCenterX->setText(QString::number(p.x()));
    lineScaleCenterY->setText(QString::number(p.y()));
}

void MainWindow::clickedScaleRelativeToOrigin() {
    lineScaleCenterX->setText(QString::number(0));
    lineScaleCenterY->setText(QString::number(0));
}

void MainWindow::clickedRotateRelativeToObject() {
    QPoint p = canvas->getFigureCenter();
    lineRotateCenterX->setText(QString::number(p.x()));
    lineRotateCenterY->setText(QString::number(p.y()));
}

void MainWindow::clickedRotateRelativeToOrigin() {
    lineRotateCenterX->setText(QString::number(0));
    lineRotateCenterY->setText(QString::number(0));
}

void MainWindow::clickedApplyMove() {
    QPoint p;

    bool ok;
    p.setX(lineMoveDx->text().toInt(&ok));
    if (ok) p.setY(lineMoveDy->text().toInt(&ok));

    if (!ok)
        QMessageBox::critical(this, "Ошибка переноса", "Должны быть введены целые числа");
    else
        canvas->apply(new ActionMove(p.x(), p.y()));
}

void MainWindow::clickedApplyScale() {
    qreal kx;
    qreal ky;
    QPoint center;

    bool ok;
    kx = lineScaleKx->text().toDouble(&ok);
    if (ok) ky = lineScaleKy->text().toDouble(&ok);

    if (!ok) {
        QMessageBox::critical(this, "Ошибка масштабирования", "Должны быть введены действительные коэффициенты");
    }
    else {
        center.setX(lineScaleCenterX->text().toInt(&ok));
        if (ok) center.setY(lineScaleCenterY->text().toInt(&ok));

        if (!ok) {
            QMessageBox::critical(this, "Ошибка масштабирования", "Должны быть введены целые координаты центра масштабирования");
        }
        else if (qFuzzyIsNull(kx) || qFuzzyIsNull(ky)) {
            int v = QMessageBox::warning(this, "Предупреждение",
                "В качестве одного из коэффициентов масштабирования выбран нуль."
                "Фигура на экране будет вырождена в отрезок. Вы уверены, что хотите продолжить?",
                QMessageBox::StandardButton::Ok, QMessageBox::StandardButton::Abort);

            ok = v == QMessageBox::StandardButton::Ok;
        }

        if (ok) canvas->apply(new ActionScale(kx, ky, center));
    }
}

void MainWindow::clickedApplyRotate() {
    qreal angle;
    QPoint center;

    bool ok;
    angle = lineRotateAngle->text().toDouble(&ok);

    if (!ok) {
        QMessageBox::critical(this, "Ошибка поворота", "Должен быть введён действительный угол в градусах");
    }
    else {
        center.setX(lineRotateCenterX->text().toInt(&ok));
        if (ok) center.setY(lineRotateCenterY->text().toInt(&ok));

        if (!ok)
            QMessageBox::critical(this, "Ошибка поворота", "Должны быть введены целые координаты центра вращения");
        else
            canvas->apply(new ActionRotate(angle, center));
    }
}

void MainWindow::clickedUndo() {
    if (!canvas->undo()) {
        QMessageBox::critical(this, "Ошибка отмены", "Не было совершено действий, которые можно было бы отменить");
    }
}
