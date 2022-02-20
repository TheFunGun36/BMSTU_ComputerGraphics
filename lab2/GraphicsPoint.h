#pragma once
#include "IGraphicsObject.h"

class GraphicsPoint : public IGraphicsObject {
public:
    GraphicsPoint(QPoint point, QPen pen = QPen(), QBrush brush = QBrush());

    virtual void addToScene(QGraphicsScene &scene) const override;
    virtual void move(const ActionMove &a) override;
    virtual void scale(const ActionScale &a) override;
    virtual void rotate(const ActionRotate &a) override;

    inline QPoint get() { return point; }

private:
    QPoint point;
};
