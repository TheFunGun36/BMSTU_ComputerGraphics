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
    ActionMove(int dx, int dy) {
        type = Type::Move;
        _dx = dx;
        _dy = dy;
    }

    inline int dx() const { return _dx; };
    inline int dy() const { return _dy; };

private:
    int _dx;
    int _dy;
};

class ActionScale : public Action {
public:
    ActionScale(qreal kx, qreal ky, QPoint center = QPoint(0, 0)) {
        type = Type::Scale;
        _kx = kx;
        _ky = ky;
        _center = center;
    }

    inline qreal kx() const { return _kx; };
    inline qreal ky() const { return _ky; };
    inline QPoint center() const { return _center; };

private:
    qreal _kx;
    qreal _ky;
    QPoint _center;
};

class ActionRotate : public Action {
public:
    ActionRotate(qreal angle, QPoint center = QPoint(0, 0)) {
        type = Type::Rotate;
        _angle = qDegreesToRadians(angle);
        _center = center;
        _cosAngle = qCos(_angle);
        _sinAngle = qSin(_angle);
    }

    inline qreal angle() const { return _angle; };
    inline qreal angleSin() const { return _sinAngle; };
    inline qreal angleCos() const { return _cosAngle; };
    inline QPoint center() const { return _center; };

private:
    qreal _angle;
    qreal _sinAngle;
    qreal _cosAngle;
    QPoint _center;
};
