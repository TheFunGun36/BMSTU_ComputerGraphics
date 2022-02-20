#include "GraphicsGroup.h"

GraphicsGroup::~GraphicsGroup() {
    for (IGraphicsObject *object : objects)
        delete object;
}

void GraphicsGroup::addToScene(QGraphicsScene &scene) const {
    for (IGraphicsObject *object : objects)
        object->addToScene(scene);
}

void GraphicsGroup::addObject(IGraphicsObject *object) {
    objects.push_back(object);
}

void GraphicsGroup::clear() {
    for (IGraphicsObject *object : objects)
        delete object;

    objects.clear();
}

void GraphicsGroup::move(const ActionMove &a) {
    for (IGraphicsObject *object : objects)
        object->move(a);
}

void GraphicsGroup::scale(const ActionScale &a) {
    for (IGraphicsObject *object : objects)
        object->scale(a);
}

void GraphicsGroup::rotate(const ActionRotate &a) {
    for (IGraphicsObject *object : objects)
        object->rotate(a);
}
