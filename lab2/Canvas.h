#pragma once
#include <qgraphicsview.h>
#include <qcolor.h>
#include <qlist.h>

#include "Action.h"
#include "GraphicsGroup.h"
#include "GraphicsPoint.h"

class Canvas : public QGraphicsView {
    Q_OBJECT

public:
    Canvas(QWidget *parent = nullptr);
    void apply(const Action &action);
    void undo();
    QPointF getFigureCenter();

private:
    QGraphicsScene scene;
    int сellSize;
    QColor colorBackground;
    QColor colorAxes;
    QColor colorGrid;
    QColor colorGridAccent;
    QList<const Action *> actions;
    GraphicsGroup figure;
    GraphicsPoint *figureCenter;

    void constructFigure();
    void recalculateScene();
};

