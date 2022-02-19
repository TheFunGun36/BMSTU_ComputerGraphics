#include "triangle.h"
#include <qvector2d>

Triangle::Triangle() {
    for (int i = 0; i < 3; i++)
        vertex[i] = QPointF(0.0, 0.0);
    area = 0;
    valid = false;
}

Triangle::Triangle(QPointF p1, QPointF p2, QPointF p3) {
    vertex[0] = p1;
    vertex[1] = p2;
    vertex[2] = p3;

    validate();
    if (valid) {
        calculateArea();
        calculateBisect();
    }
}

void Triangle::getVertexes(QPointF &p1, QPointF &p2, QPointF &p3) const {
    p1 = vertex[0];
    p2 = vertex[1];
    p3 = vertex[2];
}

void Triangle::getBisectSideOverlap(QPointF &p1, QPointF &p2, QPointF &p3) const {
    p1 = bisectSideOverlap[0];
    p2 = bisectSideOverlap[1];
    p3 = bisectSideOverlap[2];
}

QPointF Triangle::getBisectOverlap() const {
    return bisectOverlap;
}

double Triangle::getArea() const {
    return area;
}

bool Triangle::isValid() const {
    return valid;
}

std::shared_ptr<Triangle> Triangle::getBisectLargestSubtriangle() {
    if (!bisectLargestSubtriangle)
        calculateBisectSubtriangles();

    return bisectLargestSubtriangle;
}

std::shared_ptr<Triangle> Triangle::getBisectSmallestSubtriangle() {
    if (!bisectSmallestSubtriangle)
        calculateBisectSubtriangles();

    return bisectSmallestSubtriangle;
}

void Triangle::validate() {
    Line l(vertex[0], vertex[1]);
    valid = !l.doesContain(vertex[2]);
}

void Triangle::calculateBisect() {
#define CYC3(val) (((val) + 1 >= 3) ? (0) : ((val) + 1))
    Line side[3];
    Line bisec[3];

    for (int i = 0; i < 3; i++) {
        int j = CYC3(i);
        int k = CYC3(j);
        side[i] = Line(vertex[j], vertex[k]);
    }

    for (int i = 0; i < 3; i++) {
        int j = CYC3(i);
        int k = CYC3(j);

        bisec[i] = Line::bisec(side[k], side[j]);
        if (Line::isParallel(bisec[i], side[i])) {
            bisec[i] = Line::bisec(side[k], side[j]);
        }
        bisectSideOverlap[i] = Line::intercept(side[i], bisec[i]);
    }

    bisectOverlap = Line::intercept(bisec[0], bisec[1]);
#undef CYC3
}

void Triangle::calculateArea() {
    double a = QVector2D(vertex[0] - vertex[1]).length();
    double b = QVector2D(vertex[0] - vertex[2]).length();
    double c = QVector2D(vertex[1] - vertex[2]).length();
    double p = (a + b + c) / 2;
    area = sqrt(p * (p - a) * (p - b) * (p - c));
}

void Triangle::calculateBisectSubtriangles() {
#define CYC3(val) (((val) + 1 >= 3) ? (0) : ((val) + 1))
#define BCYC3(val) (((val) - 1 < 0) ? (2) : ((val) - 1))
    Triangle subtr[6];

    for (int i = 0; i < 3; i++) {
        subtr[i] = Triangle(vertex[i], bisectSideOverlap[CYC3(i)], bisectOverlap);
        subtr[i + 3] = Triangle(vertex[i], bisectSideOverlap[BCYC3(i)], bisectOverlap);
    }

    Triangle subMin = subtr[0];
    Triangle subMax = subtr[0];
    int is;
    for (is = 1; is < 6; is++) {
        if (subMin.area > subtr[is].area)
            subMin = subtr[is];
    }

    for (int i = (is == 0) ? 1 : 0; i < 6; i++) {
        if (subMax.area < subtr[i].area && i != is)
            subMax = subtr[i];
    }

    bisectLargestSubtriangle = std::make_shared<Triangle>();
    *bisectLargestSubtriangle = subMax;
    bisectSmallestSubtriangle = std::make_shared<Triangle>();
    *bisectSmallestSubtriangle = subMin;

#undef CYC3
#undef BCYC3
}
