#include "AbstractFigure.h"

void AbstractFigure::applyAction(IAction action) {
    for (auto i = points.begin(); i != points.end(); i++)
        *i = action.apply(*i);
}

void AbstractFigure::reverseAction(IAction action) {
    for (auto i = points.begin(); i != points.end(); i++)
        *i = action.reverse(*i);
}
