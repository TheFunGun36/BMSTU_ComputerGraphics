#pragma once
#include <qgraphicsview.h>
#include <qcolor.h>

class Canvas : public QGraphicsView {
    Q_OBJECT

public:
    Canvas(QWidget *parent = nullptr);
    int getScale();

public slots:
    void windowResized();

private:
    QGraphicsScene scene;

    double margin;
    int minCellSize;
    int maxCellSize;
    double scale;
    QColor colorBackground;
    QColor colorAxes;
    QColor colorGrid;
    QColor colorGridAccent;
    QRectF screenPosition;

    QPointF mathToScreen(QPointF point) const;
    QPointF screenToMath(QPointF point) const;
    void recalculateScreenPosition();

    //void drawAxes();
    //void drawGrid();
    //void drawBackground();
};

