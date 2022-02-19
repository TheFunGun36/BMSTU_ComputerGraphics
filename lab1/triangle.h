#pragma once
#include "line.h"
#include <qpoint>
#include <memory>

class Triangle {
public:
    Triangle();
    Triangle(QPointF p1, QPointF p2, QPointF p3);
    void getVertexes(QPointF &p1, QPointF &p2, QPointF &p3) const;
    void getBisectSideOverlap(QPointF &p1, QPointF &p2, QPointF &p3) const;
    QPointF getBisectOverlap() const;
    double getArea() const;
    bool isValid() const;

    std::shared_ptr<Triangle> getBisectLargestSubtriangle();
    std::shared_ptr<Triangle> getBisectSmallestSubtriangle();

private:
    bool valid;
    double area;
    QPointF vertex[3];
    QPointF bisectSideOverlap[3];
    QPointF bisectOverlap;

    std::shared_ptr<Triangle> bisectLargestSubtriangle;
    std::shared_ptr<Triangle> bisectSmallestSubtriangle;

    void validate();
    void calculateBisect();
    void calculateArea();
    void calculateBisectSubtriangles();
};

