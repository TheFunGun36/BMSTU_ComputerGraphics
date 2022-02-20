#pragma once
#include "IGraphicsObject.h"
#include <qpolygon.h>
#include "Action.h"

class GraphicsPolygon : public IGraphicsObject {
public:
    GraphicsPolygon(const QPolygon &polygon, QPen pen = QPen(), QBrush brush = QBrush());
    virtual void addToScene(QGraphicsScene &scene) const override;

    virtual void move(const ActionMove &a) override;
    virtual void scale(const ActionScale &a) override;
    virtual void rotate(const ActionRotate &a) override;

private:
    QPolygon polygon;
};

