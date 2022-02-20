#pragma once
#include "IGraphicsObject.h"
#include <qlist.h>

class GraphicsGroup : public IGraphicsObject {
public:
    ~GraphicsGroup();
    virtual void addToScene(QGraphicsScene &scene) const override;
    void addObject(IGraphicsObject *object);
    void clear();

    virtual void move(const ActionMove &a) override;
    virtual void scale(const ActionScale &a) override;
    virtual void rotate(const ActionRotate &a) override;

private:
    QList<IGraphicsObject *> objects;
};

