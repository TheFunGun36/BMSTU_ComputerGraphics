#include "GraphicsPoint.h"
#include <qgraphicsitem.h>

GraphicsPoint::GraphicsPoint(QPoint point, QPen pen, QBrush brush) {
    this->point = point;
    this->pen = pen;
    this->brush = brush;
}

void GraphicsPoint::addToScene(QGraphicsScene &scene) const {
    const int r = 5;
    const int d = 2 * r;
    QRect rect(point.x() - r, point.y() - r, d, d);
    scene.addEllipse(rect, pen, brush);
    QGraphicsTextItem *textItem = scene.addText(QString("(%1, %2)").arg(point.x()).arg(point.y()));
    textItem->setPos(point);
}

void GraphicsPoint::move(const ActionMove &a) {
    point.setX(point.x() + a.dx());
    point.setY(point.y() + a.dy());
}

void GraphicsPoint::scale(const ActionScale &a) {
    point.setX(qRound(point.x() * a.kx() + (1 - a.kx()) * a.center().x()));
    point.setY(qRound(point.y() * a.ky() + (1 - a.ky()) * a.center().y()));
}

void GraphicsPoint::rotate(const ActionRotate &a) {
    int dx = point.x() - a.center().x();
    int dy = point.y() - a.center().y();
    point.setX(qRound(a.center().x() + dx * a.angleCos() + dy * a.angleSin()));
    point.setY(qRound(a.center().y() - dx * a.angleSin() + dy * a.angleCos()));
}
