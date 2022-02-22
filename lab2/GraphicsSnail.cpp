#include "GraphicsSnail.h"
#include <qmath.h>
#include <qvector2d.h>

GraphicsSnail::GraphicsSnail(qreal a, qreal b, QPoint center, QPen pen, QBrush brush) {
    ax = a;
    bx = b;
    cx = 0.0;
    dx = 0.0;
    ay = 0.0;
    by = 0.0;
    cy = a;
    dy = b;
    this->center = center;
    this->pen = pen;
    this->brush = brush;
}

void GraphicsSnail::addToScene(QGraphicsScene &scene) const {
    QPolygon p;

    // dt поодбрано так, чтобы максимальное расстояние между точками было близко к 1
    qreal largestCoef = qMax(
        qMax(qMax(qAbs(ax), qAbs(bx)), qMax(qAbs(cx), qAbs(dx))),
        qMax(qMax(qAbs(ay), qAbs(by)), qMax(qAbs(cy), qAbs(dy))));
    qreal dt = M_PI / (largestCoef / 2);
    for (qreal t = 0; t < 2 * M_PI; t += dt) {
        p.push_back(QPoint(x(t), y(t)));
    }

    scene.addPolygon(p, pen, brush);
}

void GraphicsSnail::move(const ActionMove &a) {
    center.setX(center.x() + a.dx());
    center.setY(center.y() + a.dy());
}

void GraphicsSnail::scale(const ActionScale &a) {
    ax = a.kx() * ax;
    bx = a.kx() * bx;
    cx = a.kx() * cx;
    dx = a.kx() * dx;

    ay = a.ky() * ay;
    by = a.ky() * by;
    cy = a.ky() * cy;
    dy = a.ky() * dy;

    center.setX(qRound(a.kx() * center.x() + (1 - a.kx()) * a.center().x()));
    center.setY(qRound(a.ky() * center.y() + (1 - a.ky()) * a.center().y()));
}

void GraphicsSnail::rotate(const ActionRotate &a) {
    qreal nax = ax * a.angleCos() + ay * a.angleSin();
    qreal nbx = bx * a.angleCos() + by * a.angleSin();
    qreal ncx = cx * a.angleCos() + cy * a.angleSin();
    qreal ndx = dx * a.angleCos() + dy * a.angleSin();

    qreal nay = -ax * a.angleSin() + ay * a.angleCos();
    qreal nby = -bx * a.angleSin() + by * a.angleCos();
    qreal ncy = -cx * a.angleSin() + cy * a.angleCos();
    qreal ndy = -dx * a.angleSin() + dy * a.angleCos();

    ax = nax;
    bx = nbx;
    cx = ncx;
    dx = ndx;

    ay = nay;
    by = nby;
    cy = ncy;
    dy = ndy;

    QPoint c = center;
    int deltaX = c.x() - a.center().x();
    int deltaY = c.y() - a.center().y();

    center.setX(qRound(a.center().x() + deltaX * a.angleCos() + deltaY * a.angleSin()));
    center.setY(qRound(a.center().y() - deltaX * a.angleSin() + deltaY * a.angleCos()));
}
