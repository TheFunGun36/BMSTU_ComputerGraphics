#include "Canvas.h"
#include <qpainter.h>
#include "GraphicsPolygon.h"
#include "GraphicsSnail.h"

constexpr int maxDist = 4000;
constexpr int gridStep = 50;

Canvas::Canvas(QWidget *parent)
    : scene(this), QGraphicsView(parent) {
    setScene(&scene);

    colorBackground = QColor(255, 255, 255);
    colorAxes = QColor(0, 100, 150);
    colorGrid = QColor(0, 200, 200);
    colorGridAccent = QColor(0, 50, 150);
    figureCenter = nullptr;

    scene.setBackgroundBrush(colorBackground);

    constructFigure();
    recalculateScene();
}

void Canvas::apply(const Action *action) {
    figure.applyAction(*action);
    actions.push_back(action);
    recalculateScene();
}

bool Canvas::undo() {
    if (actions.isEmpty())
        return false;

    delete actions.last();
    actions.pop_back();

    constructFigure();
    for (const Action *a : actions)
        figure.applyAction(*a);
    recalculateScene();

    return true;
}

void Canvas::reset() {
    actions.clear();
    constructFigure();
    recalculateScene();
}

QPointF Canvas::getFigureCenter() {
    return figureCenter->get();
}

void Canvas::constructFigure() {
    constexpr qreal rhombusShort = 150.0;
    constexpr qreal rhombusLong = 200.0;
    constexpr qreal snailA = 100.0;
    constexpr qreal snailB = 50.0;

    figure.clear();

    // Ромб
    {
        QPolygonF poly({
            { 0.0, rhombusShort },
            { rhombusLong, 0.0 },
            { 0.0, -rhombusShort },
            { -rhombusLong, 0.0}
            });
        QPen pen(QColor(200, 20, 20), 2);
        QBrush brush(QColor(200, 20, 20, 50));
        figure.addObject(new GraphicsPolygon(poly, pen, brush));
    }

    // Улитка Паскаля
    {
        QPen pen(QColor(20, 200, 20), 2);
        QBrush brush(QColor(20, 200, 20, 50));
        GraphicsSnail *snail = new GraphicsSnail(snailA, snailB, QPointF(-rhombusLong / 4, 0), pen, brush);
        figure.addObject(snail);
    }

    // Центральная точка
    {
        QPen pen(QColor(0, 0, 0));
        QBrush brush(QColor(0, 0, 0));
        figureCenter = new GraphicsPoint(QPointF(0, 0), pen, brush);
        figure.addObject(figureCenter);
    }
}

void Canvas::recalculateScene() {
    scene.clear();
    for (int i = gridStep; i < maxDist; i += gridStep) {
        scene.addLine(-maxDist, i, maxDist, i, colorGrid);
        scene.addLine(-maxDist, -i, maxDist, -i, colorGrid);
        scene.addLine(i, -maxDist, i, maxDist, colorGrid);
        scene.addLine(-i, -maxDist, -i, maxDist, colorGrid);
    }

    scene.addLine(-maxDist, 0, maxDist, 0, QPen(colorAxes, 3));
    scene.addLine(0, -maxDist, 0, maxDist, QPen(colorAxes, 3));

    figure.addToScene(scene);
}
