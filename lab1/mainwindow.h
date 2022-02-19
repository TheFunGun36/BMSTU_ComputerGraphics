#pragma once

#include <qmainwindow>
#include <qtablewidget>
#include <qpushbutton>
#include <qspinbox>
#include <qlabel>
#include "canvas.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    Canvas *canvas;
    QTableWidget *pointsTable;
    QSpinBox *spinRemovePoint;
    QPushButton *buttonAddPoint;
    QPushButton *buttonRemovePoint;
    QLabel *labelScale;
    QLineEdit *outSmallTriangle;
    QLineEdit *outBigTriangle;
    QLineEdit *outAnswer;

    void resizeEvent(QResizeEvent *event);

public slots:
    void buttonAddPointClicked();
    void buttonRemovePointClicked();
    void updateScale(double scale);
    void setNewAnswer(double smallTriag, double bigTriag);
};
