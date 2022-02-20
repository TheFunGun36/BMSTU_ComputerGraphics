#include "GraphicsPolygon.h"

GraphicsPolygon::GraphicsPolygon(const QPolygon &polygon, QPen pen, QBrush brush) {
    this->polygon = polygon;
    this->pen = pen;
    this->brush = brush;
}

void GraphicsPolygon::addToScene(QGraphicsScene &scene) const {
    scene.addPolygon(polygon, pen, brush);
}

void GraphicsPolygon::move(const ActionMove &a) {
    for (QPoint &point : polygon) {
        point.setX(point.x() + a.dx());
        point.setY(point.y() + a.dy());
    }
}

void GraphicsPolygon::scale(const ActionScale &a) {
    for (QPoint &point : polygon) {
        point.setX(qRound(point.x() * a.kx() + (1 - a.kx()) * a.center().x()));
        point.setY(qRound(point.y() * a.ky() + (1 - a.ky()) * a.center().y()));
    }
}

void GraphicsPolygon::rotate(const ActionRotate &a) {
    for (QPoint &point : polygon) {
        int dx = point.x() - a.center().x();
        int dy = point.y() - a.center().y();
        point.setX(qRound(a.center().x() + dx * a.angleCos() + dy * a.angleSin()));
        point.setY(qRound(a.center().y() - dx * a.angleSin() + dy * a.angleCos()));
    }
}
