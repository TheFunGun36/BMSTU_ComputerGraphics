#include "GraphicsPoint.h"

GraphicsPoint::GraphicsPoint(QPointF point, QPen pen, QBrush brush) {
    this->point = point;
    this->pen = pen;
    this->brush = brush;
}

void GraphicsPoint::addToScene(QGraphicsScene &scene) const {
    const int r = 10;
    QRect rect(point.x() - r, point.y() - r, point.x() + r, point.y() + r);
    scene.addEllipse(rect, pen, brush);
}

void GraphicsPoint::move(const ActionMove &a) {
    point.setX(point.x() + a.dx());
    point.setY(point.y() + a.dy());
}

void GraphicsPoint::scale(const ActionScale &a) {
    point.setX(point.x() * a.kx() + (1 - a.kx()) * a.center().x());
    point.setY(point.y() * a.ky() + (1 - a.ky()) * a.center().y());
}

void GraphicsPoint::rotate(const ActionRotate &a) {
    qreal dx = point.x() - a.center().x();
    qreal dy = point.y() - a.center().y();
    point.setX(a.center().x() + dx * a.angleCos() + dy * a.angleSin());
    point.setY(a.center().y() - dx * a.angleSin() + dy * a.angleCos());
}
