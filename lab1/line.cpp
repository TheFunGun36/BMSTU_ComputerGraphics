#include "line.h"

Line::Line() {
    a = 0;
    b = 0;
    c = 0;
}

Line::Line(QPointF p1, QPointF p2) {
    a = p2.y() - p1.y();
    b = p1.x() - p2.x();
    c = -(a * p1.x() + b * p1.y());

    /*if (a < 0) {
        a = -a;
        b = -b;
        c = -c;
    }*/
}

Line::Line(QPointF p, QVector2D v) {
    a = v.y();
    b = -v.x();
    c = -(a * p.x() + b * p.y());

    /*if (a < 0) {
        a = -a;
        b = -b;
        c = -c;
    }*/
}

QPointF Line::intercept(const Line &l1, const Line &l2) {
    QPointF result;

    if (isParallel(l1, l2)) {
        throw Exception::linesAreParallel;
    }
    else if (qFuzzyIsNull(l1.a)) {
        if (qFuzzyIsNull(l1.b)) {
            throw Exception::lineInvalid;
        }
        else {
            result.setY(-l1.c / l1.b);
            result.setX(-(l2.b * result.y() + l2.c) / l2.a);
        }
    }
    else if (qFuzzyIsNull(l2.a)) {
        if (qFuzzyIsNull(l2.b)) {
            throw Exception::lineInvalid;
        }
        else {
            result.setY(-l2.c / l2.b);
            result.setX(-(l1.b * result.y() + l1.c) / l1.a);
        }
    }
    else {
        double numerator = l2.a * l1.c / l1.a - l2.c;
        double denominator = l2.b - l2.a * l1.b / l1.a;
        result.setY(numerator / denominator);
        result.setX(-(l1.b * result.y() + l1.c) / l1.a);
    }
    return result;
}

Line Line::bisec(const Line &l1, const Line &l2) {
    QPointF point = intercept(l1, l2);
    QVector2D vector = l1.getVector().normalized() + l2.getVector().normalized();
    return Line(point, vector);
}

bool Line::isParallel(const Line &l1, const Line &l2) {
    bool result = false;

    result = qFuzzyIsNull(l1.a) && qFuzzyIsNull(l2.a);
    result = result || qFuzzyIsNull(l1.b) && qFuzzyIsNull(l2.b);
    result = result || qFuzzyCompare(l1.a * l2.b, l1.b * l2.a);

    return result;
}

bool Line::doesContain(const QPointF point) {
    return qFuzzyIsNull(a * point.x() + b * point.y() + c);
}

QVector2D Line::getVector() const {
    return QVector2D(a, b);
}
