#include "GraphicsPolygon.h"

GraphicsPolygon::GraphicsPolygon(const QPolygonF &polygon, QPen pen, QBrush brush) {
    this->polygon = polygon;
    this->pen = pen;
    this->brush = brush;
}

void GraphicsPolygon::addToScene(QGraphicsScene &scene) const {
    scene.addPolygon(polygon, pen, brush);
}

void GraphicsPolygon::move(const ActionMove &a) {
    for (QPointF &point : polygon) {
        point.setX(point.x() + a.dx());
        point.setY(point.y() + a.dy());
    }
}

void GraphicsPolygon::scale(const ActionScale &a) {
    for (QPointF &point : polygon) {
        point.setX(point.x() * a.kx() + (1 - a.kx()) * a.center().x());
        point.setY(point.y() * a.ky() + (1 - a.ky()) * a.center().y());
    }
}

void GraphicsPolygon::rotate(const ActionRotate &a) {
    for (QPointF &point : polygon) {
        qreal dx = point.x() - a.center().x();
        qreal dy = point.y() - a.center().y();
        point.setX(a.center().x() + dx * a.angleCos() + dy * a.angleSin());
        point.setY(a.center().y() - dx * a.angleSin() + dy * a.angleCos());
    }
}
