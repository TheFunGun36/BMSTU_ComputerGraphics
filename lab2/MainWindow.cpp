#include "mainwindow.h"
#include <qlineedit.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qvalidator.h>

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
    leftPanel->addWidget(cancel);

    leftPanel->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    layout->addLayout(leftPanel);
    layout->addWidget(canvas);
}

void MainWindow::createMoveSection(QVBoxLayout *layout) {
    QLabel *title = new QLabel("Перенос", this);
    title->setFont(QFont("Times New Roman", 18));
    layout->addWidget(title);

    QLineEdit *lineDx = new QLineEdit(this);
    QLineEdit *lineDy = new QLineEdit(this);

    QIntValidator *validator = new QIntValidator(this);
    lineDx->setValidator(validator);
    lineDy->setValidator(validator);

    auto layoutData = new QGridLayout(this);
    layoutData->addWidget(new QLabel("Смещение по x:", this), 0, 0);
    layoutData->addWidget(new QLabel("Смещение по y:", this), 1, 0);
    layoutData->addWidget(lineDx, 0, 1);
    layoutData->addWidget(lineDy, 1, 1);

    layout->addLayout(layoutData);

    QPushButton *apply = new QPushButton("Перенести", this);
    layout->addWidget(apply);
};

void MainWindow::createScaleSection(QVBoxLayout *layout) {
    QLabel *title = new QLabel("Масштаб", this);
    title->setFont(QFont("Times New Roman", 18));
    layout->addWidget(title);

    QLineEdit *linekx = new QLineEdit(this);
    QLineEdit *lineky = new QLineEdit(this);
    QLineEdit *linecx = new QLineEdit(this);
    QLineEdit *linecy = new QLineEdit(this);
    linecx->setPlaceholderText("x");
    linecy->setPlaceholderText("y");

    QIntValidator *validator = new QIntValidator(this);
    linekx->setValidator(validator);
    lineky->setValidator(validator);
    linecx->setValidator(validator);
    linecy->setValidator(validator);

    auto layoutData = new QGridLayout(this);
    layoutData->addWidget(new QLabel("Коэффициент x:", this), 0, 0);
    layoutData->addWidget(new QLabel("Коэффициент y:", this), 1, 0);
    layoutData->addWidget(linekx, 0, 1);
    layoutData->addWidget(lineky, 1, 1);

    layout->addLayout(layoutData);

    layout->addWidget(new QLabel("Центр масштабирования:", this));
    auto layoutCenter = new QHBoxLayout(this);
    layoutCenter->addWidget(linecx);
    layoutCenter->addWidget(linecy);
    layout->addLayout(layoutCenter);

    QPushButton *useObjCenter = new QPushButton("Относительно центра фигуры", this);
    QPushButton *useOrigin = new QPushButton("Относительно начала координат", this);
    layout->addWidget(useObjCenter);
    layout->addWidget(useOrigin);

    QPushButton *apply = new QPushButton("Масштабировать", this);
    layout->addWidget(apply);
};

void MainWindow::createRotateSection(QVBoxLayout *layout) {
    QLabel *title = new QLabel("Поворот", this);
    title->setFont(QFont("Times New Roman", 18));
    layout->addWidget(title);

    QLineEdit *lineangle = new QLineEdit(this);
    QLineEdit *linecx = new QLineEdit(this);
    QLineEdit *linecy = new QLineEdit(this);
    linecx->setPlaceholderText("x");
    linecy->setPlaceholderText("y");

    QIntValidator *validator = new QIntValidator(this);
    QDoubleValidator *validatorDouble = new QDoubleValidator(this);
    validatorDouble->setLocale(QLocale("C"));
    lineangle->setValidator(validatorDouble);
    linecx->setValidator(validator);
    linecy->setValidator(validator);

    layout->addWidget(new QLabel("Угол в градусах (против часовой стрелки):", this));
    layout->addWidget(lineangle);

    layout->addWidget(new QLabel("Центр поворота:", this));
    auto layoutCenter = new QHBoxLayout(this);
    layoutCenter->addWidget(linecx);
    layoutCenter->addWidget(linecy);
    layout->addLayout(layoutCenter);

    QPushButton *useObjCenter = new QPushButton("Относительно центра фигуры", this);
    QPushButton *useOrigin = new QPushButton("Относительно начала координат", this);
    layout->addWidget(useObjCenter);
    layout->addWidget(useOrigin);

    QPushButton *apply = new QPushButton("Повернуть", this);
    layout->addWidget(apply);
};
