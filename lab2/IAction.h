#pragma once
#include <qpoint.h>

class IAction {
public:
    virtual QPointF apply(QPointF point);
    virtual QPointF reverse(QPointF point);
};
