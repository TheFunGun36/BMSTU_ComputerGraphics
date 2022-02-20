#pragma once
#include <qgraphicsscene.h>
#include "Action.h"

class IGraphicsObject {
public:
    virtual void addToScene(QGraphicsScene &scene) const abstract;
    inline void applyAction(const Action &action) {
        switch (action.getType()) {
            case Action::Type::Move:
                move(static_cast<const ActionMove &>(action));
                break;
            case Action::Type::Scale:
                scale(static_cast<const ActionScale &>(action));
                break;
            case Action::Type::Rotate:
                rotate(static_cast<const ActionRotate &>(action));
                break;
        }
    }

    virtual void move(const ActionMove &a) abstract;
    virtual void scale(const ActionScale &a) abstract;
    virtual void rotate(const ActionRotate &a) abstract;

protected:
    QPen pen;
    QBrush brush;
};

