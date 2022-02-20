#pragma once
#include "IGraphicsObject.h"

class GraphicsSnail : public IGraphicsObject {
public:
    GraphicsSnail(qreal a,
        qreal b,
        QPoint center = QPoint(),
        QPen pen = QPen(),
        QBrush brush = QBrush());

    virtual void addToScene(QGraphicsScene &scene) const override;
    virtual void move(const ActionMove &a) override;
    virtual void scale(const ActionScale &a) override;
    virtual void rotate(const ActionRotate &a) override;

private:
    inline qreal x(qreal t) const {
        qreal cost = qCos(t);
        qreal sint = qSin(t);
        return center.x()
            + ax * cost * cost
            + bx * cost
            + cx * cost * sint
            + dx * sint;
    }
    inline qreal y(qreal t) const {
        qreal cost = qCos(t);
        qreal sint = qSin(t);
        return center.y()
            + ay * cost * cost
            + by * cost
            + cy * cost * sint
            + dy * sint;
    }

    qreal ax;
    qreal bx;
    qreal cx;
    qreal dx;
    qreal ay;
    qreal by;
    qreal cy;
    qreal dy;
    QPoint center;
};

