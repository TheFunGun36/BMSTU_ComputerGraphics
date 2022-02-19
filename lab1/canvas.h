#pragma once
#include <qwidget>
#include <qtablewidget>
#include <qcolor>
#include <list>
#include "triangle.h"

class Canvas : public QWidget {
    Q_OBJECT

public:
    Canvas(QWidget *parent = nullptr);
    void setPointsTable(QTableWidget *tablePoints);
    int getScale();

    enum class Exception {
        cantReadCell,
    };

public slots:
    void pointsTableChanged();
    void windowResized();

signals:
    void scaleChanged(double scale);
    void newAnswer(double smallTriag, double bigTriag);

private:
    double margin;
    int pointRadius;
    int minCellSize;
    int maxCellSize;
    double scale;
    QColor colorBackground;
    QColor colorPoint;
    QColor colorPointTaskSecondary;
    QColor colorPointTask;
    QColor colorAxes;
    QColor colorGrid;
    QColor colorGridAccent;
    QColor colorTriangleBig;
    QColor colorTriangleSmall;
    QRectF screenPosition;
    QTableWidget *tablePoints;

    QPointF mathToScreen(QPointF point) const;
    QPointF screenToMath(QPointF point) const;
    void recalculateScreenPosition();
    void drawPoint(QPainter *qp, QPointF point);
    void drawPoints(QPainter *qp);
    void drawAxes(QPainter *qp);
    void drawGrid(QPainter *qp);
    void drawTriangles(QPainter *qp);
    void drawBackground(QPainter *qp);
    void paintEvent(QPaintEvent *event);

    QVector<QPointF> pointsTask;
    QVector<QPointF> pointsTaskSecondary;
    QVector<QPointF> points;
    Triangle taskTriangle;

    QPointF getPoint(int index) const;
    void taskCalculate();
    double taskCalculateTriangle(Triangle &triangle) const;
};

