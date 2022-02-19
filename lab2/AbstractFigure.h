#pragma once
#include <qlist.h>
#include <qpoint.h>
#include <qgraphicsscene.h>
#include "IAction.h"

class AbstractFigure {
public:
    virtual void addToScene(QGraphicsScene &scene) abstract;
    void applyAction(IAction action);
    void reverseAction(IAction action);
protected:
    QList<QPointF> points;
};

