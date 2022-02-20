#pragma once
#include "IGraphicsObject.h"

class GraphicsPoint : public IGraphicsObject {
public:
    GraphicsPoint(QPointF point, QPen pen = QPen(), QBrush brush = QBrush());

    virtual void addToScene(QGraphicsScene &scene) const override;
    virtual void move(const ActionMove &a) override;
    virtual void scale(const ActionScale &a) override;
    virtual void rotate(const ActionRotate &a) override;

private:
    QPointF point;
};
