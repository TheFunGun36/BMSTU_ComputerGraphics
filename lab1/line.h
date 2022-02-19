#pragma once
#include <qpoint>
#include <qexception>
#include <qvector2d>

class Line {
public:
    Line();
    Line(QPointF p1, QPointF p2);
    Line(QPointF p, QVector2D v);
    static QPointF intercept(const Line &l1, const Line &l2);
    static Line bisec(const Line &l1, const Line &l2);
    static bool isParallel(const Line &l1, const Line &l2);
    bool doesContain(const QPointF point);
    QVector2D getVector() const;

    enum class Exception {
        linesAreParallel,
        lineInvalid
    };

private:
    double a, b, c;
};
