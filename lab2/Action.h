#pragma once
#include <qpoint.h>
#include <qmath.h>

class Action {
public:
    enum class Type {
        None,
        Move,
        Scale,
        Rotate
    };

    inline Type getType() const { return type; }

protected:
    Type type = Type::None;
};

class ActionMove : public Action {
public:
    ActionMove(qreal dx, qreal dy) {
        type = Type::Move;
        _dx = dx;
        _dy = dy;
    }

    inline qreal dx() const { return _dx; };
    inline qreal dy() const { return _dy; };

private:
    qreal _dx;
    qreal _dy;
};

class ActionScale : public Action {
public:
    ActionScale(qreal kx, qreal ky, QPointF center = QPoint(0, 0)) {
        type = Type::Scale;
        _kx = kx;
        _ky = ky;
        _center = center;
    }

    inline qreal kx() const { return _kx; };
    inline qreal ky() const { return _ky; };
    inline QPointF center() const { return _center; };

private:
    qreal _kx;
    qreal _ky;
    QPointF _center;
};

class ActionRotate : public Action {
public:
    ActionRotate(qreal angle, QPointF center = QPointF(0, 0)) {
        type = Type::Rotate;
        _angle = angle;
        _center = center;
        _cosAngle = qCos(angle);
        _sinAngle = qSin(angle);
    }

    inline qreal angle() const { return _angle; };
    inline qreal angleSin() const { return _sinAngle; };
    inline qreal angleCos() const { return _cosAngle; };
    inline QPointF center() const { return _center; };

private:
    qreal _angle;
    qreal _sinAngle;
    qreal _cosAngle;
    QPointF _center;
};
